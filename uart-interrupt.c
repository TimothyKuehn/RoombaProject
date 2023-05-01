/*
 *
 *   uart-interrupt.c
 *
 *
 *
 *   @author
 *   @date
 */

// The "???" placeholders should be the same as in your uart.c file.
// The "?????" placeholders are new in this file and must be replaced.
#include <inc/tm4c123gh6pm.h>
#include <stdint.h>
#include <string.h>
#include "uart-interrupt.h"
#include "lcd.h"
#include <cyBot_Scan.h>
#include "movement.h"

// These variables are declared as examples for your use in the interrupt handler.
volatile char command_byte = 0; // byte value for special character used as a command
volatile int command_flag = 0; // flag to tell the main program a special command was received
volatile bool startup = false;
volatile bool flag_m = false;
volatile bool flag_s = false;
volatile bool flag_r = false;
volatile bool flag_1 = false;
volatile bool flag_2 = false;
volatile bool flag_3 = false;
volatile bool flag_4 = false;
volatile bool flag_5 = false;
volatile bool flag_6 = false;
void uart_interrupt_init(void)
{
    //TODO
    //enable clock to GPIO port B
    SYSCTL_RCGCGPIO_R |= 0x2;

    //enable clock to UART1
    SYSCTL_RCGCUART_R |= 0x2;

    //wait for GPIOB and UART1 peripherals to be ready
    while ((SYSCTL_PRGPIO_R & 0x2) == 0)
    {
    };
    while ((SYSCTL_PRUART_R & 0x2) == 0)
    {
    };

    //enable digital functionality on port B pins
    GPIO_PORTB_DEN_R |= 0x3;

    //enable alternate functions on port B pins
    GPIO_PORTB_AFSEL_R |= 0x03;

    //enable UART1 Rx and Tx on port B pins
    GPIO_PORTB_PCTL_R &= 0xEE;
    GPIO_PORTB_PCTL_R |= 0x11;

    //calculate baud rate
    uint16_t iBRD = 8; //use equations
    uint16_t fBRD = 44; //use equations

    //turn off UART1 while setting it up
    UART1_CTL_R &= 0xCFE;

    //set baud rate
    //note: to take effect, there must be a write to LCRH after these assignments
    UART1_IBRD_R = iBRD;
    UART1_FBRD_R = fBRD;

    //set frame, 8 data bits, 1 stop bit, no parity, no FIFO
    //note: this write to LCRH must be after the BRD assignments
    UART1_LCRH_R &= 0x0;
    UART1_LCRH_R = 0x60; //was 0b01100000

    //use system clock as source
    //note from the datasheet UARTCCC register description:
    //field is 0 (system clock) by default on reset
    //Good to be explicit in your code
    UART1_CC_R = 0x0;

    //////Enable interrupts

    //first clear RX interrupt flag (clear by writing 1 to ICR)
    UART1_ICR_R |= 0b00010000;

    //enable RX raw interrupts in interrupt mask register
    UART1_IM_R |= 0x10;

    //NVIC setup: set priority of UART1 interrupt to 1 in bits 21-23
    NVIC_PRI1_R = (NVIC_PRI1_R & 0xFF0FFFFF) | 0x00200000;

    //NVIC setup: enable interrupt for UART1, IRQ #6, set bit 6
    NVIC_EN0_R |= 0x40;

    //tell CPU to use ISR handler for UART1 (see interrupt.h file)
    //from system header file: #define INT_UART1 22
    IntRegister(INT_UART1, UART1_Handler);

    //globally allow CPU to service interrupts (see interrupt.h file)
    IntMasterEnable();

    //re-enable UART1 and also enable RX, TX (three bits)
    //note from the datasheet UARTCTL register description:
    //RX and TX are enabled by default on reset
    //Good to be explicit in your code
    //Be careful to not clear RX and TX enable bits
    //(either preserve if already set or set them)
    UART1_CTL_R = 0x301;

}

void uart_sendChar(char data)
{

    while (UART1_FR_R & 0x20)
    {
    }

    UART1_DR_R = data;

}

char uart_receive(void)
{
    char received;

    while (UART1_FR_R & UART_FR_RXFE)
    {
    }
    received = (char) (UART1_DR_R & 0xFF);

    return received;
}

void uart_sendStr(const char *data)
{
    int i = 0;
    for (i = 0; i < strlen(data); ++i)
    {
        uart_sendChar(data[i]);
    }
    uart_sendChar('\n');
}

char uart_receive_nonblocking(void)
{
    char received;

    UART1_FR_R & ~UART_FR_RXFE;
    received = (char) (UART1_DR_R & 0xFF);

    return received;
}

// Interrupt handler for receive interrupts
void UART1_Handler(void)
{
    char byte_received = command_byte;
    //check if handler called due to RX event
    if (UART1_MIS_R & 0x10)
    {
        //byte was received in the UART data register
        //clear the RX trigger flag (clear by writing 1 to ICR)
        UART1_ICR_R |= 0b00010000;

        //read the byte received from UART1_DR_R and echo it back to PuTTY
        //ignore the error bits in UART1_DR_R
        byte_received = (char) (UART1_DR_R & 0xFF);
//        uart_sendChar(byte_received);

        //if byte received is a carriage return

        if (byte_received == 'm')
        {
            flag_m = true;
        }
        if (byte_received == 's')
        {
            flag_s = true;
        }
        if (byte_received == 'r')
        {
            flag_r = true;
        }
        if (byte_received == '1')
        {
            flag_1 = true;
        }
        if (byte_received == '2')
        {
            flag_2 = true;
        }
        if (byte_received == '3')
        {
            flag_3 = true;
        }
        if (byte_received == '4')
        {
            flag_4 = true;
        }
        if (byte_received == '5')
        {
            flag_5 = true;
        }
        if (byte_received == '6')
        {
            flag_6 = true;
        }

        //AS NEEDED
        //code to handle any other special characters
        //code to update global shared variables
        //DO NOT PUT TIME-CONSUMING CODE IN AN ISR
        command_byte = byte_received;
        command_flag = 1;

    }
}
