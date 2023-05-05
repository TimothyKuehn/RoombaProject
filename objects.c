/**
 * 
 * objects.c
 * 
 * Created Feb 18, 2023
 *    @author mdwells
 * 
 **/

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

// Structure for representing detected objects
typedef struct
{
float distance;
int angle;
float width;
} Objects;

// Function for sending the array of detected objects over UART
void sendObjectArr(Objects *arr, int numObjects)
{
    // Create a string array
    char objects[10][50];
    int i;
    for (i = 0; i < numObjects; ++i)
    {
        float dist = arr[i].distance;
        int angle = arr[i].angle;
        float rad = arr[i].width;
        // Create a string for the object and put into the sting array
        sprintf(objects[i], "%d!%.2f!%.2f", angle, dist, rad); 
    }


    char st[200] = "I"; // Trigger identifier "I"
    for (i = 0; i < numObjects; ++i)
    {
        char object[50];
        strcpy(object, objects[i]); // Pull a single object from the string array
        strcat(st, object); // Concatenate the object onto st

        // Checks if there is another object to add
        if (numObjects - i > 1)
        {
            char end[] = "@"; 
            strcat(st, end); // Concatenate an indicator for a new object in st
        }
    }

    printf(st);
    uart_sendStr(st);
}

// function for identifying objects in the scanner's field of view
void identifyObjects(float inputDistances[])
{
    flag_s = false; // Reset the flag
    int j;
    float distances[180];
    
    // Invert and scale the IR values
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
        if ((distances[i] - distances[i + 1] > 20) && // If the next angle has a signifigantly lower distance
            (!inObj) && // If not currently in an object
            distances[i + 1]) // If there is a next angle
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
                    float objectDistance = scannerPING(angle);
                    objectArr[numObjects].distance = objectDistance;

                    //radial width
                    float width = 6.28 * objectDistance * (end - start) / (360.0);
                    objectArr[numObjects].width = width;
					
                    numObjects = numObjects + 1; //Number of objects detected
                }
            
                // Reset object boolean and start angle
                inObj = false;
                start = 0;
            }
        }
    }
    
    // Send the object array to the GUI
    if (numObjects > 0)
    {
       sendObjectArr(objectArr,numObjects);
    }
    
    // Send an indicator that no objects were found
    if (numObjects < 1){
       uart_sendStr("K");
    }
}