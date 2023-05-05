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
#include "music.h"

int main(void)
{
    // Initialize various components and sensors
    timer_init(); // Initialize timer
    oi_t *sensor_data = oi_alloc(); // Allocate memory for sensor data
    oi_init(sensor_data); // Initialize the sensor data
    lcd_init(); // Initialize LCD display
    adc_init(); // Initialize analog-to-digital converter
    uart_interrupt_init(); // Initialize UART interrupt
    servo_init(); // Initialize servo motor
    ping_init(); // Initialize ultrasonic sensor

    // Variables for tracking distance moved and angle turned
    float distanceMoved;
    float angleTurned;
    
    // Print a pirate greeting to the LCD
    char startupMessage[] = "AHOY MATEY!";
    lcd_printf(startupMessage);

    while (1)
    {
        distanceMoved = 0; // Reset distanceMoved to 0
        angleTurned = 0; // Reset angleTurned to 0

        // Check if a flag has been set to move forward
        if (flag_m)
        {
            distanceMoved = move_forward(sensor_data, 100); // Move forward 100 units
            char returnString[20];
            sprintf(returnString, "D0!%f", distanceMoved); // Format a string with the distance moved
            uart_sendStr(returnString); // Send the string over UART
            flag_m = false; // Reset the flag
        }
        
        // Check if a flag has been set to move backward
        if (flag_r)
        {
            distanceMoved = move_backward(sensor_data, 50); // Move backward 50 units
            char returnString[20];
            sprintf(returnString, "D0!%lf", distanceMoved); // Format a string with the distance moved
            uart_sendStr(returnString); // Send the string over UART
            flag_r = false; // Reset the flag
        }

        // Check if a flag has been set to turn left at 90 degrees
        if (flag_1)
        {
            angleTurned = turn_left(sensor_data, 90); // Turn left 90 degrees
            char returnString[20];
            sprintf(returnString, "D%f!0", angleTurned); // Format a string with the angle turned
            uart_sendStr(returnString); // Send the string over UART
            flag_1 = false; // Reset the flag
        }

        // Check if a flag has been set to turn left at 45 degrees
        if (flag_2)
        {
            angleTurned = turn_left(sensor_data, 45); // Turn left 45 degrees
            char returnString[20];
            sprintf(returnString, "D%f!0", angleTurned); // Format a string with the angle turned
            uart_sendStr(returnString); // Send the string over UART
            flag_2 = false; // Reset the flag
        }

        // Check if a flag has been set to turn left at 15 degrees
        if (flag_3)
        {
            angleTurned = turn_left(sensor_data, 15); // Turn left 15 degrees
            char returnString[20];
            sprintf(returnString, "D%f!0", angleTurned); // Format a string with the angle turned
            uart_sendStr(returnString); // Send the string over UART
            flag_3 = false; // Reset the flag
        }

        // Check if a flag has been set to perform an IR scan
        if (flag_s)
        {
            scannerIR(); // Perform an IR scan
            identifyObjects(rawVal); // Identify objects in the scan
        }
        
        // Check if a flag has been set to turn right 15 degrees
        if (flag_4)
        {
            angleTurned = turn_right(sensor_data, 15); // Turn right 15 degrees
            char returnString[20]; 
            sprintf(returnString, "D%f!0", angleTurned); // Format a string with the angle returned
            uart_sendStr(returnString); //Send the string over UART
            flag_4 = false; // Reset the flag
        }
        
        // Check if a flag has been set to turn right 45 degrees
        if (flag_5)
        {
            angleTurned = turn_right(sensor_data, 45); // Turn right 45 degrees
            char returnString[20];
            sprintf(returnString, "D%f!0", angleTurned); // Format a string with the angle returned
            uart_sendStr(returnString); // Send the string over UART
            flag_5 = false; // Reset the string
        }
        
        //Check if a flag has been set to turn right 90 degrees
        if (flag_6)
        {
            angleTurned = turn_right(sensor_data, 90); // Turn right 90 degrees
            char returnString[20];
            sprintf(returnString, "D%f!0", angleTurned); // Format a string with the angle returned
            uart_sendStr(returnString); // Send the string over UART
            flag_6 = false; // Reset the flag

        }
        
        // Check if a flag has been set to play a song 
        if (flag_c)
        {
            load_songs();
            play_songs(1); // Play song number 1 "Wellerman" Sea Shanty
            flag_c = false; // Reset the flag
        }
    }
}