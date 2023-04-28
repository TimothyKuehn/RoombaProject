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



//Moves the bot some distance
void move_forward(oi_t *sensor_data, double distance_mm){

     double sum = 0;
     oi_setWheels(100,100); //move forward slowly
     while (sum < distance_mm) {
       oi_update(sensor_data);
       sum += sensor_data -> distance; //accumulate distance
     }

     if(     (sensor_data -> bumpRight) ||
             (sensor_data -> bumpLeft) ||
             (sensor_data -> cliffLeft) ||
             (sensor_data -> cliffFrontLeft) ||
             (sensor_data -> cliffFrontRight) ||
             (sensor_data -> cliffRight))
     {
         stop(sensor_data);
     }


     oi_setWheels(0,0); //stop
}

//Moves the bot some distance
void move_backward(oi_t *sensor_data, double distance_mm){

     double sum = 0;
     oi_setWheels(-100,-100); //move forward slowly
     while (sum < distance_mm) {
       oi_update(sensor_data);
       sum -= sensor_data -> distance; //accumulate distance
     }
     oi_setWheels(0,0); //stop
}

//stops the bot and backs up to scan
void stop(oi_t *sensor_data){
          oi_setWheels(0,0); //stop
          timer_waitMillis(500); //Wait half a second to prevent movement into hole.
          
}

void turn_right(oi_t *sensor, double degrees){
    oi_setWheels(-100,100);
    double sum = 0;
    while (sum < degrees) {
          oi_update(sensor);
          sum -= sensor -> angle; // use -> notation since pointer
}
    oi_setWheels(0,0); //stop
}

void turn_left(oi_t *sensor, double degrees) {
    oi_setWheels(100,-100);
    double sum = 0;
     while (sum < degrees) {
           oi_update(sensor);
           sum += sensor -> angle; // use -> notation since pointer
 }
     oi_setWheels(0,0); //stop
 }



