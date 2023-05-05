/*
 * movement.c
 *
 *  Created on: Feb 3, 2023
 *      Author: mdwells
 */

#include "open_interface.h"
#include "movement.h"
#include "lcd.h"
#include "timer.h"
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <inc/tm4c123gh6pm.h>
#include <uart-interrupt.h>

// Moves the bot forward a set distance
double move_forward(oi_t *sensor_data, double distance_mm)
{
    char string[3];
    double sum = 0;
    int speed = 100;
    oi_setWheels(speed, speed); // move forward slowly
	
    while (sum < distance_mm)
    {
        oi_update(sensor_data);
        sum += sensor_data->distance; //accumulate distance
		
		//if statements to stop the bot and send the GUI information if any of the cliff/bump sensors are triggered
        if (sensor_data->cliffFrontLeft)
        {
            stop();
            sprintf(string, "C1"); //indicates to the GUI which sensor was trigger
            uart_sendStr(string); 
            return sum;
        }
        if (sensor_data->cliffFrontRight)
        {
            stop();
            sprintf(string, "C2");
            uart_sendStr(string);
            return sum;
        }
        if (sensor_data->bumpRight)
        {
            stop();
            sprintf(string, "B1");
            uart_sendStr(string);
            return sum;
        }
        if (sensor_data->bumpLeft)
        {
            stop();
            sprintf(string, "B0");
            uart_sendStr(string);
            return sum;
        }
        if (sensor_data->cliffLeft)
        {
            stop();
            sprintf(string, "C0");
            uart_sendStr(string);
            return sum;
        }

        if (sensor_data->cliffRight)
        {
            stop();
            sprintf(string, "C3");
            uart_sendStr(string);
            return sum;
        }
        if (sensor_data->cliffFrontLeftSignal > 2700)
        {
            stop();
            sprintf(string, "L1");
            uart_sendStr(string);
            return sum;
        }
        if (sensor_data->cliffFrontRightSignal > 2700)
        {
            stop();
            sprintf(string, "L2");
            uart_sendStr(string);
            return sum;
        }
        if (sensor_data->cliffLeftSignal > 2700)
        {
            stop();
            sprintf(string, "L0");
            uart_sendStr(string);
            return sum;
        }

        if (sensor_data->cliffRightSignal > 2700)
        {
            stop();
            sprintf(string, "L3");
            uart_sendStr(string);
            return sum;
        }
    }

    oi_setWheels(0, 0); //stop

    return sum;
}

//Moves the bot some distance
double move_backward(oi_t *sensor_data, double distance_mm)
{

    double sum = 0;
    oi_setWheels(-100, -100); //move forward slowly
    while (sum < distance_mm)
    {
        oi_update(sensor_data);
        sum -= sensor_data->distance; //accumulate distance
    }
    oi_setWheels(0, 0); //stop
    return (-1 * sum);
}

//stops the bot
void stop()
{
    oi_setWheels(0, 0); //stop
    timer_waitMillis(500); //Wait half a second to prevent movement into hole.

}

// Turns the bot right some degrees
double turn_right(oi_t *sensor, double degrees)
{
    oi_setWheels(-50, 50);
    double sum = 0;
    while (sum < degrees)
    {
        oi_update(sensor);
        sum -= sensor->angle; // use -> notation since pointer
    }
    oi_setWheels(0, 0); //stop
    return sum;
}

// Turns the bot left some degrees
double turn_left(oi_t *sensor, double degrees)
{
    oi_setWheels(50, -50);
    double sum = 0;
    while (sum < degrees)
    {
        oi_update(sensor);
        sum += sensor->angle; // use -> notation since pointer
    }
    oi_setWheels(0, 0); //stop
    return -1 * sum;
}

