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
#include "uart-interrupt.h"
#include "movement.h"
#include "Scanner.h"
#include "ping.h"

typedef struct
{
    float distance;
    int angle;
    float width;
} Objects;

void sendObjectArr(Objects *arr, int numObjects)
{

    char objects[10][50];
    int i;
    for (i = 0; i < numObjects; ++i)
    {
        float dist = arr[i].distance;
        int angle = arr[i].angle;
        float rad = arr[i].width;
        sprintf(objects[i], "%d!%.2f!%.2f", angle, dist, rad);
    }

    char st[200] = "I";
    for (i = 0; i < numObjects; ++i)
    {
       // int j;
     //   for(j = 0; j< strlen(objects[i]); ++j){
            char object[50];
            strcpy(object, objects[i]);
            strcat(st, object);

        if (numObjects - i > 1)
        {
            char end[] = "@";
            strcat(st, end);
        }
    }

    printf(st);
    uart_sendStr(st);

}


void identifyObjects(float inputDistances[])
{
    flag_s = false;
    int j;
    float distances[180];
    for (j = 0; j < 180; j++)
    {
        distances[j] = 100000 / (*(inputDistances + j));
    }

    Objects objectArr[20];
    int numObjects = 0;
    int i;
    int start = 0;
    int end = 0;
    bool inObj = false;
    for (i = 0; i < 179; ++i)
    {

        //detect first edge
        if ((distances[i] - distances[i + 1] > 20) && (!inObj) && distances[i + 1]) // may need to change the 90 once on the test field
        {
            start = i;
            inObj = true;
        }

        //detect second edge
        if (start > 0)
        {
            if ((distances[i + 1] - distances[i] > 10) && (inObj))
            {

                end = i;
                if ((end - start) > 8)
                {

                    //calculate average angle of object
                    int angle = (start + end) / 2;
                    objectArr[numObjects].angle = angle;

                    //distance of object
                    float objectDistance = scannerPING(angle); //fails here sometime, check after sendObjectArr is fixed.
                    objectArr[numObjects].distance = objectDistance;

                    //radial width
                    float width = 6.28 * objectDistance * (end - start) / (360.0);
                    objectArr[numObjects].width = width;
                    numObjects = numObjects + 1;
                }
                inObj = false;
                start = 0;
            }
        }
    }

    if (numObjects > 0)
    {
        sendObjectArr(objectArr,numObjects);
    }
    if (numObjects < 1){
        uart_sendStr("K");
    }
}
