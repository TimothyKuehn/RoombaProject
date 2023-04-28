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
#include "cyBot_Scan.h"




struct Objects{
     float distance;
     int angle;
     float radialWidth;
 };



void printToPutty(float distance, int i) {

        //cyBot_sendByte("angle\t");
        //cyBot_sendByte(i);
        //char space[] = " ";
        //cyBot_sendByte(space);
        //printf("%d ",i);

        //prints the angle in putty
        char ang[5];
        sprintf(ang,"%d",i);
        int len = strlen(ang);
        int h;
        for(h = 0; h < len; ++h){
            cyBot_sendByte(ang[h]);
            printf("%c",ang[h]);
        }

        //prints a tab to the putty
        int g;
        char tab[] = "\t";
        for(g = 0; g < 2; ++g){
        cyBot_sendByte(tab[g]);
        }
        printf("\t");


        //prints the distance in putty
        char dis[10];
        sprintf(dis,"%.2f",distance);
        int l = strlen(dis);
        int j;
        for(j = 0; j < l; ++j){
            cyBot_sendByte(dis[j]);
            printf("%c",dis[j]);
        }

        //sends a newline and return to the putty
        int k;
        char newLine[] = "\n\r";
        for(k = 0; k < 4; ++k){
            cyBot_sendByte(newLine[k]);
        }
        //cyBot_sendByte("\n");
        printf("\n");

    }


void printObjectArr(struct Objects* arr, int numObjects) {

    char objects[numObjects][100];
    int i;
    for (i = 0; i < numObjects; ++i){
        float dist = arr[i].distance;
        int angle = arr[i].angle;
        float rad = arr[i].radialWidth;
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









