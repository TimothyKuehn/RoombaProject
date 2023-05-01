/*
 * objects.c
 *
 *  Created on: Feb 18, 2023
 *      Author: mdwells
 */

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include "Timer.h"
#include <inc/tm4c123gh6pm.h>
#include "lcd.h"
#include "open_interface.h"
#include "movement.h"
//#include "Scanner.h"

typedef struct {
    float distance;
    int angle;
    float width;
}Objects;


void identifyObjects(float distances[], Objects *objectArr) {

     int numObjects = sizeof(objectArr);
     int i;
     int start = 0;
     int end = 0;
     _Bool inObj = false;
     for(i = 0; i < 89; ++i){

         //detect first edge
         if((distances[i] - distances[i + 1] > 10) && (inObj == false)) {
            start = i;
            inObj = true;
         }

         //detect second edge
         if((distances[i + 1] - distances[i] > 10) &&
             (start > 0) &&
             (inObj == true))
         {
             end = i;
             inObj = false;

             //calculate average angle of object
             double angle = start + end;
             objectArr[numObjects].angle = angle;

             //distance of object
             float objectDistance = scannerPING(angle);
             objectArr[numObjects].distance = objectDistance;

             //radial width
             float width = 4 * 3.14 * objectDistance * ((end - start) / 360.0);
             objectArr[numObjects].width = width;
             if(width > 5){
                 numObjects = numObjects + 1;
             }
         }
     }
 }

void sendObjectArr(Objects* arr) {

    int numObjects = sizeof(arr);
    char objects[numObjects][100];
    int i;
    for (i = 0; i < numObjects; ++i){
        float dist = arr[i].distance;
        int angle = arr[i].angle;
        float rad = arr[i].width;
        sprintf(objects[i], "I%d!%d!%d", angle, dist, rad);
    }

    char st[] = "I";
    for ( i = 0; i < numObjects; ++i) {
        char object[50];
        strcpy(object,objects[i]);
        strcat(st,object);
        if(numObjects - i > 1){
            char end[] = "@";
            strcat(st,end);
        }
    }

    printf(st);
    uart_sendStr(st);

}
