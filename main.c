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
#include "Timer.h"
#include <inc/tm4c123gh6pm.h>
#include "lcd.h"
#include "open_interface.h"
#include "movement.h"
#include "objects.h"
#include "cyBot_Scan.h"

 struct Objects{
     float distance;
     int angle;
     float radialWidth;
  };

 struct Objects arr_objects[180];

 int numObjects = 0;



 void identifyObjects(float distances[]) {

     cyBOT_Scan_t getScan;
     int i;
     int start = 0;
     int end = 0;
     _Bool obj = false;
     for(i = 0; i < 89; ++i){

         //detect first edge
         if((distances[i] - distances[i + 1] > 30) && (obj == false)) {
            start = i;
            obj = true;
         }

         //detect second edge
         if((distances[i + 1] - distances[i] > 30) &&
             (start > 0) &&
             (obj == true)){

             end = i;
             obj = false;

             //calculate average angle of object
             double angle = start + end;
             arr_objects[numObjects].angle = angle;

             //distance of object
             int j;
             float objectDistance = 0;
             cyBOT_Scan(angle, &getScan);
             timer_waitMillis(500);
             for(j = 0; j < 3; ++j){
             cyBOT_Scan(angle, &getScan);
             objectDistance += getScan.sound_dist;
             }
             objectDistance = objectDistance / 3;

             arr_objects[numObjects].distance = objectDistance;

             //radial width
             float width = 4 * 3.14 * objectDistance * ((end - start) / 360.0);
             arr_objects[numObjects].radialWidth = width;
             if(width > 5){
             numObjects = numObjects + 1;
             }
         }
     }
 }



 void clearObjects(){
     int i;
     for(i = 0; i < 180; ++i){
         arr_objects[i].distance = 0;
         arr_objects[i].radialWidth = 0;
         arr_objects[i].angle = 0;
     }
 }





int main(void) {
 oi_t *sensor_data = oi_alloc(); // do this only once at start of main()
 oi_init(sensor_data); // do this only once at start of main()

 timer_init();
 lcd_init();
 cyBot_uart_init();
 cyBOT_init_Scan(0b0111);
 uart_interrupt_init();
 //cyBOT_SERVO_cal();

 cyBOT_Scan_t getScan;
 right_calibration_value = 232750;
 left_calibration_value = 1251250;



float distances[91];


 while(1){

     //cyBot scan, print values, identify objects
     if(scan_flag == 1) {
         int i;
         int j;
         numObjects = 0;
         cyBOT_Scan(0,&getScan);
         timer_waitMillis(500);
         for(i = 0; i <= 180 ; i = i + 2){
             float avg_distance = 0;
             for(j = 0; j < 3; ++j){
                 cyBOT_Scan(i, &getScan);
                 float distance = (100000 / getScan.IR_raw_val);
                 avg_distance = avg_distance + distance;
             }
             avg_distance = avg_distance / 3;

             printToPutty(avg_distance,i);
             distances[(i/2)] = avg_distance;
         }

         clearObjects();
         identifyObjects(distances);
         printObjectArr(arr_objects,numObjects);
         oi_free(sensor_data);
      }

     if(stop_flag == 1) {
         oi_setWheels(0,0);
         stop_flag == 0;
     }

     if(forward_flag == 1){
         oi_update(sensor_data);
         move_forward(sensor_data, 10);
         char forwardData[10];
         sprintf(forwardData, "%f", sensor_data->distance);
         uart_sendStr(forwardData);
         forward_flag == 0;
     }

     if(back_flag == 1){
         oi_update(sensor_data);
         move_backward(sensor_data, 10);
         char backData[10];
         sprintf(backData, "%f", sensor_data->distance);
         uart_sendStr(backData);
         back_flag == 0;
     }

     if(right_flag == 1){
         oi_update(sensor_data);
         turn_right(sensor_data, 10);
         char rightData[10];
         sprintf(rightData, "%f", sensor_data->distance);
         uart_sendStr(rightData);
         right_flag == 0;
     }

     if(left_flag == 1){
         oi_update(sensor_data);
         turn_left(sensor_data, 10);
         char leftData[10];
         sprintf(leftData, "%f", sensor_data->distance);
         uart_sendStr(leftData);
         left_flag == 0;
     }




     }



    oi_free(sensor_data);  // do this once at end of main()
 return 0;
 }


     //oi_free(sensor_data);
   //  return 0;
