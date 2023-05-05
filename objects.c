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

void sendObjectArr(Objects *arr, int numObjects) //This function formats the objects data to a single string and sends to Java GUI
{

    char objects[10][50]; //Create 2D array for object's data
    int i;
    
	//Move object data to an array of strings
	for (i = 0; i < numObjects; ++i)
    {
        float dist = arr[i].distance;
        int angle = arr[i].angle;
        float rad = arr[i].width;
        sprintf(objects[i], "%d!%.2f!%.2f", angle, dist, rad); //Move string to 2D array
    }

    char st[200] = "I"; //I is the indicator for GUI to know this is the data of a detected object
    for (i = 0; i < numObjects; ++i)
    {
            char object[50];
            strcpy(object, objects[i]);
            strcat(st, object); // concatenate the object onto "I" indicator

        if (numObjects - i > 1)
        {
            char end[] = "@"; 
            strcat(st, end); //If there is more than 1 object add "@" in between the object's data
        }
    }

    printf(st); 
    uart_sendStr(st); // Sends object's data to GUI

}


void identifyObjects(float inputDistances[]) // Object detection
{
    flag_s = false; //Resets the scan flag
    int j;
    float distances[180];
	
	// Moves inputDistances[] to distances array and scales the inverse IR value for easier use in object detection
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
        if ((distances[i] - distances[i + 1] > 20) && (!inObj) && distances[i + 1]) // If there a drop in IR value from previous IR value greater than 20 set that as first edge.
        {
            start = i; //Object starts at angle i
            inObj = true;
        }

        //detect second edge
        if (start > 0) //Object can't start at first angle
        {
            if ((distances[i + 1] - distances[i] > 10) && (inObj)) //if the IR value goes up by more than 10 from the previous IR value set as end of object.
            {

                end = i; //Object ends at angle i
                
				if ((end - start) > 8) //Dectected object must be greater than a total angle of 8 to be consider a real object.
                {

                    //calculate average angle of object
                    int angle = (start + end) / 2;
                    objectArr[numObjects].angle = angle;

                    //distance of object
                    float objectDistance = scannerPING(angle); //Uses ping at objects location to get accurate distance
                    objectArr[numObjects].distance = objectDistance;

                    //radial width
                    float width = 6.28 * objectDistance * (end - start) / (360.0);
                    objectArr[numObjects].width = width;
					
                    numObjects = numObjects + 1; //Number of objects detected
                }
                inObj = false; //Set's inObj to false so new object can be detected
                start = 0;
            }
        }
    }

    if (numObjects > 0) // If any objects were detected, send formated data to GUI
    {
        sendObjectArr(objectArr,numObjects);
    }
    if (numObjects < 1) // Sends a "K" to let GUI scan completed with zero objects detected.
	{ 
        uart_sendStr("K");
    }
}
