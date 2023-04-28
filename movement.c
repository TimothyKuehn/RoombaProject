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



//Moves the bot some distance
double move_forward(oi_t *sensor_data, double distance_mm){
     char string[3];
     double dist = 0;
     oi_setWheels(100,100); //move forward slowly
     while (dist < distance_mm) {
       oi_update(sensor_data);
       dist += sensor_data -> distance; //accumulate distance
     }

     if(sensor_data -> bumpRight){
         stop(sensor_data);
         string[3] = "B1";
         uart_sendStr(string);
     }
     if(sensor_data -> bumpLeft){
         stop(sensor_data);
         string[3] = "B0";
         uart_sendStr(string);
          }
     if(sensor_data -> cliffLeft){
         stop(sensor_data);
         string[3] = "C0";
         uart_sendStr(string);
          }
     if(sensor_data -> cliffFrontLeft){
         stop(sensor_data);
         string[3] = "C1";
         uart_sendStr(string);
          }
     if(sensor_data ->cliffFrontRight){
         stop(sensor_data);
         string[3] = "C2";
         uart_sendStr(string);
          }
     if(sensor_data -> cliffRight){
         stop(sensor_data);
         string[3] = "C3";
         uart_sendStr(string);
          }
     if(sensor_data -> cliffLeftSignal > 2000){
         stop(sensor_data);
         string[3] = "L0";
         uart_sendStr(string);
     }
     if(sensor_data -> cliffFrontLeftSignal > 2000){
         stop(sensor_data);
         string[3] = "L1";
         uart_sendStr(string);
     }
     if(sensor_data -> cliffFrontRightSignal > 2000){
         stop(sensor_data);
         string[3] = "L2";
         uart_sendStr(string);
     }
     if(sensor_data -> cliffRightSignal > 2000){
         stop(sensor_data);
         string[3] = "L3";
         uart_sendStr(string);
     }


     oi_setWheels(0,0); //stop

     return sensor_data ->distance;
}

//Moves the bot some distance
double move_backward(oi_t *sensor_data, double distance_mm){

     double sum = 0;
     oi_setWheels(-100,-100); //move forward slowly
     while (sum < distance_mm) {
       oi_update(sensor_data);
       sum -= sensor_data -> distance; //accumulate distance
     }
     oi_setWheels(0,0); //stop
     return sensor_data -> distance;
}

//stops the bot and backs up to scan
void stop(oi_t *sensor_data){
          oi_setWheels(0,0); //stop
          timer_waitMillis(500); //Wait half a second to prevent movement into hole.
          
}

double turn_right(oi_t *sensor, double degrees){
    oi_setWheels(-100,100);
    double sum = 0;
    while (sum < degrees) {
          oi_update(sensor);
          sum -= sensor -> angle; // use -> notation since pointer
}
    oi_setWheels(0,0); //stop
    return sensor -> angle;
}

double turn_left(oi_t *sensor, double degrees) {
    oi_setWheels(100,-100);
    double sum = 0;
     while (sum < degrees) {
           oi_update(sensor);
           sum += sensor -> angle; // use -> notation since pointer
 }
     oi_setWheels(0,0); //stop
     return sensor -> angle;
 }



