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
        sprintf(objects[i], "I%d!%f!%f", angle, dist, rad);
    }

    char st[] = "I";
    for (i = 0; i < numObjects; ++i)
    {
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
void clearObjects(Objects *arr)
{
    int i;
    for (i = 0; i < 180; ++i)
    {
        arr[i].distance = 0;
        arr[i].width = 0;
        arr[i].angle = 0;
    }
}
void printObjectArr(Objects *arr, int numObjects) {

    int i;
    for (i = 0; i < numObjects; ++i){
        float dist = arr[i].distance;
        int angle = arr[i].angle;
        float rad = arr[i].width;

        printf("Object %d\n",i+1);
        printf("Distance %.2f\n",dist);
        printf("Angle %d\n",angle);
        printf("Radial Width %.2f\n",rad);
        printf("\n");

        //prints object number
        char num[2];
        sprintf(num,"%d",i + 1);
        int b =0;
        for(b = 0; b < 2; ++b){
            uart_sendChar(num[b]);
        }

        //prints a tab to the putty
        int z;
        char tab[] = "\t";
        for(z = 0; z < 2; ++z){
            uart_sendChar(tab[z]);
        }
        printf("\t");

        //prints the distance in putty
                char dis[10];
                sprintf(dis,"%.2f",dist);
                int l = strlen(dis);
                int j;
                for(j = 0; j < l; ++j){
                    uart_sendChar(dis[j]);
                    printf("%c",dis[j]);
                }

                //prints a tab to the putty
                int g;
                char tab2[] = "\t";
                for(g = 0; g < 2; ++g){
                    uart_sendChar(tab2[g]);
                }
                printf("\t");

        //angle to putty
        char ang[5];
        sprintf(ang,"%d",angle);
        int len = strlen(ang);
        int h;
        for(h = 0; h < len; ++h){
            uart_sendChar(ang[h]);
            printf("%c",ang[h]);
        }

        //prints a tab to the putty
        int v;
        char tab3[] = "\t";
        for(v = 0; v < 2; ++v){
            uart_sendChar(tab3[v]);
        }
        printf("\t");

        //prints the radialWidth in putty
                char rw[10];
                sprintf(rw,"%.2f",rad);
                int x = strlen(rw);
                int c;
                for(c = 0; c < x; ++c){
                    uart_sendChar(rw[c]);
                    printf("%c",rw[c]);
                }

                //sends a newline and return to the putty
                       int k;
                       char newLine[] = "\n\r";
                       for(k = 0; k < 4; ++k){
                           uart_sendChar(newLine[k]);
                       }
                       //cyBot_sendByte("\n");
                       printf("\n");
    }

}

void identifyObjects(float inputDistances[]){
    flag_s = false;
    int j;
    float distances[180];
    for (j = 0; j < 180; j++)
    {
        distances[j] = 100000 / (*(inputDistances + j));
    }

    Objects objectArr[20];
    clearObjects(objectArr);
    int numObjects = 0;
    int i;
    int start = 0;
    int end = 0;
    _Bool inObj = false;
    for (i = 0; i < 179; ++i)
    {

        //detect first edge
        if ((distances[i] - distances[i + 1] > 10) && (!inObj)&& distances[i+1]<90){
            start = i;
            inObj = true;
        }

        //detect second edge
        if (start > 0){
            if ((distances[i + 1] - distances[i] > 10) && (inObj)){



                end = i;
                if ((end - start) > 9){

                    //calculate average angle of object
                    int angle = (start + end)/2;
                    objectArr[numObjects].angle = angle;

                    //distance of object
                    float objectDistance = IRDist(angle); //fails here sometime, check after sendObjectArr is fixed.
                    objectArr[numObjects].distance = objectDistance;

                    //radial width
                    float width = 2 * M_PI *  objectDistance * (end - start) /(360.0 - (M_PI * (end - start)));
                    objectArr[numObjects].width = width;
                        numObjects = numObjects + 1;
                    }
                inObj = false;
                start = 0;
             }
           }
        }


if(numObjects > 0)
{
    printObjectArr(objectArr,numObjects);
   // sendObjectArr(objectArr,numObjects); // Breaks the Program*** NEED FIXING
    }
}
