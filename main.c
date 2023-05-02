/**
 * lab6-interrupt_template.c
 *
 * Template file for CprE 288 Lab 6
 *
 * @author Diane Rover, 2/15/2020
 *
 */

#include "uart-interrupt.h"
#include "driverlib/interrupt.h"
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
#include "Scanner.h"
#include "Timer.h"
#include <inc/tm4c123gh6pm.h>
#include "lcd.h"
#include "open_interface.h"
#include "movement.h"
#include "objects.h"
#include "adc.h"
#include "servo.h"
#include "ping.h"

int main(void)
{
    timer_init(); // Must be called before lcd_init(), which uses timer functions
    oi_t *sensor_data = oi_alloc(); // do this only once at start of main()
    oi_init(sensor_data); // do this only once at start of main()
    lcd_init();
    adc_init();
    uart_interrupt_init();
    servo_init();
    ping_init();
    char startupMessage[] = "STARTUP MESSAGE";
    lcd_printf(startupMessage);
    double distanceMoved;
    double angleTurned;
    while (1)
    {
        distanceMoved = 0;
        angleTurned = 0;

        if (flag_m)
        {
            distanceMoved = move_forward(sensor_data, 100);
            char returnString[20];
            sprintf(returnString, "D0!%f", distanceMoved);
            uart_sendStr(returnString);
            flag_m = false;
        }

        if (flag_1)
        {
            angleTurned = turn_left(sensor_data, 90);
            char returnString[20];
            sprintf(returnString, "D%f!0", angleTurned);
            uart_sendStr(returnString);
            flag_1 = false;
        }
        if (flag_2)
        {
            angleTurned = turn_left(sensor_data, 45);
            char returnString[20];
            sprintf(returnString, "D%f!0", angleTurned);
            uart_sendStr(returnString);
            flag_2 = false;
        }
        if (flag_3)
        {
            angleTurned = turn_left(sensor_data, 15);
            char returnString[20];
            sprintf(returnString, "D%f!0", angleTurned);
            uart_sendStr(returnString);
            flag_3 = false;
        }
        if (flag_s)
        {
            int i = 0;
            float dist[180];
            timer_waitMillis(10);
            scannerIR();

            for(i= 0; i < 180; ++i){
                dist[i] = rawVal[i];
            }
            identifyObjects(dist);
            lcd_printf("m");

        }
        if (flag_4)
        {
            angleTurned = turn_right(sensor_data, 15);
            char returnString[20];
            sprintf(returnString, "D%f!0", angleTurned);
            uart_sendStr(returnString);
            flag_4 = false;
        }
        if (flag_5)
        {
            angleTurned = turn_right(sensor_data, 45);
            char returnString[20];
            sprintf(returnString, "D%f!0", angleTurned);
            uart_sendStr(returnString);
            flag_5 = false;
        }
        if (flag_6)
        {
            angleTurned = turn_right(sensor_data, 90);
            char returnString[20];
            sprintf(returnString, "D%f!0", angleTurned);
            uart_sendStr(returnString);
            flag_6 = false;

        }

    }

}
