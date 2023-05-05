/**
 * movement.c
 * Created on: Feb 3, 2023
 *    Author: mdwells
 * 
**/

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

    while (abs(sum) < abs(distance_mm)) { // accumulate distance
        oi_update(sensor_data);
        sum += (speed < 0 ? -1 : 1) * sensor_data->distance; // update distance based on direction
        
        // stop and return if any sensors detect an obstacle
        if (sensor_data->cliffFrontLeft || sensor_data->cliffFrontRight ||
            sensor_data->bumpRight || sensor_data->bumpLeft ||
            sensor_data->cliffLeft || sensor_data->cliffRight ||
            sensor_data->cliffFrontLeftSignal > 2700 ||
            sensor_data->cliffFrontRightSignal > 2700 ||
            sensor_data->cliffLeftSignal > 2700 ||
            sensor_data->cliffRightSignal > 2700) {
            stop();
            if (sensor_data->cliffFrontLeft) {
                sprintf(string, "C1");
            } else if (sensor_data->cliffFrontRight) {
                sprintf(string, "C2");
            } else if (sensor_data->bumpRight) {
                sprintf(string, "B1");
            } else if (sensor_data->bumpLeft) {
                sprintf(string, "B0");
            } else if (sensor_data->cliffLeft) {
                sprintf(string, "C0");
            } else if (sensor_data->cliffRight) {
                sprintf(string, "C3");
            } else if (sensor_data->cliffFrontLeftSignal > 2700) {
                sprintf(string, "L1");
            } else if (sensor_data->cliffFrontRightSignal > 2700) {
                sprintf(string, "L2");
            } else if (sensor_data->cliffLeftSignal > 2700) {
                sprintf(string, "L0");
            } else if (sensor_data->cliffRightSignal > 2700) {
                sprintf(string, "L3");
            }
            uart_sendStr(string);
            return abs(sum);
        }
    }

    oi_setWheels(0, 0); // stop
    return abs(sum);
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


//stops the bot and backs up to scan
void stop()
{
    oi_setWheels(0, 0); //stop
    timer_waitMillis(500); //Wait half a second to prevent movement into hole.

}

// Turns the bot right some degrees
double turn_right(oi_t *sensor, double degrees) 
{
    return turn(sensor, degrees, -50, 50);
}

// Turns the bot left some degrees
double turn_left(oi_t *sensor, double degrees) 
{
    return turn(sensor, degrees, 50, -50);
}

// Abstact turning function
double turn(oi_t *sensor, double degrees, int left_speed, int right_speed) 
{
    double sum = 0;
    oi_setWheels(left_speed, right_speed); // set the wheels to turn

    while (abs(sum) < abs(degrees)) { // accumulate angle
        oi_update(sensor);
        sum += (left_speed < 0 ? -1 : 1) * sensor->angle; // update angle based on direction
    }

    oi_setWheels(0, 0); // stop
    return abs(sum);
}
