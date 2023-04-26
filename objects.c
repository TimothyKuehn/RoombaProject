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

    int i;
    for (i = 0; i < numObjects; ++i){
        float dist = arr[i].distance;
        int angle = arr[i].angle;
        float rad = arr[i].radialWidth;

        printf("Object %d\n",i+1);
        printf("Distance %.2f\n",dist);
        printf("Angle %d\n",angle);
        printf("Radial Width %.2f\n",rad);
        printf("\n");

        //prints object number
        char num[2];
        sprintf(num,"%d",i + 1);
        int b;
        for(b = 0; b < 2; ++b){
        cyBot_sendByte(num[b]);
        }

        //prints a tab to the putty
        int z;
        char tab[] = "\t";
        for(z = 0; z < 2; ++z){
        cyBot_sendByte(tab[z]);
        }
        printf("\t");

        //prints the distance in putty
                char dis[10];
                sprintf(dis,"%.2f",dist);
                int l = strlen(dis);
                int j;
                for(j = 0; j < l; ++j){
                    cyBot_sendByte(dis[j]);
                    printf("%c",dis[j]);
                }

                //prints a tab to the putty
                int g;
                char tab2[] = "\t";
                for(g = 0; g < 2; ++g){
                cyBot_sendByte(tab2[g]);
                }
                printf("\t");

        //angle to putty
        char ang[5];
        sprintf(ang,"%d",angle);
        int len = strlen(ang);
        int h;
        for(h = 0; h < len; ++h){
            cyBot_sendByte(ang[h]);
            printf("%c",ang[h]);
        }

        //prints a tab to the putty
        int v;
        char tab3[] = "\t";
        for(v = 0; v < 2; ++v){
        cyBot_sendByte(tab3[v]);
        }
        printf("\t");

        //prints the radialWidth in putty
                char rw[10];
                sprintf(rw,"%.2f",rad*2);
                int x = strlen(rw);
                int c;
                for(c = 0; c < x; ++c){
                    cyBot_sendByte(rw[c]);
                    printf("%c",rw[c]);
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

}









