/*
 * scansmallest.h
 *
 *  Created on: Mar 23, 2023
 *      Author: timkuehn
 */

#include "cyBot_Scan.h"
#include "uart-interrupt.h"
#include "open_interface.h"
#include <stdbool.h>
#include "driverlib/interrupt.h"

volatile int targetAngle = 0;

double runScan(void)
{
    uart_interrupt_init();
    cyBOT_init_Scan(0b0111);

    right_calibration_value = 259000;
    left_calibration_value = 1204000;

    cyBOT_Scan_t getScan;
    char header[] = "\r\nAngle\tIR Avg\t Sonar Avg \r\n";
    uart_sendStr(header);

    int i = 0;

    int irAvg[60];
    double sonarAvg[60];

    for (i = 0; i < 180; i += 3)
    {
        int j = 0;
        int irTemp = 0;
        double sonarTemp = 0;

        for (j = 0; j < 3; j++)
        {
            cyBOT_Scan(i, &getScan);
            irTemp += getScan.IR_raw_val;
            sonarTemp += getScan.sound_dist;
        }
        irAvg[i / 3] = irTemp / 3;
        sonarAvg[i / 3] = sonarTemp / 3;
        char puTTYdata[20];
        sprintf(puTTYdata, "%i\t %i \t %f\t\n\r", i, irAvg[i / 3],
                sonarAvg[i / 3]);
        uart_sendStr(puTTYdata);
    }

    int objectStartAngle[60];
    int objectEndAngle[60];
    int objectCounter = -1;
    if (irAvg[0] > 800)
    {
        objectCounter = 0;
        objectStartAngle[objectCounter] = 0;
    }
    for (i = 1; i < 60; i++)
    {
        if (irAvg[i] > 800) //if average at i is more than 800 (close to sensor)
        {

            if ((irAvg[i + 1] > 800) && irAvg[i - 1] < 800) //if the previous value (i-1) is also close to sensor but the one before that is not (2 visible
            {
                objectCounter++;
                objectStartAngle[objectCounter] = (i - 1) * 3;
            }
            if (i == 59)
            {
                objectEndAngle[objectCounter] = 177;
            }

        }

        else if (irAvg[i] < 800)
        {
            if (irAvg[i + 1] < 800 && irAvg[i - 1] > 800)
            {
                objectEndAngle[objectCounter] = (i - 1) * 3;
            }
        }

        char puTTYdata[20];

        if ((irAvg[i] > 800 && irAvg[i + 1] > 800)
                || (irAvg[i] > 800 && irAvg[i - 1] > 800))
        {
            sprintf(puTTYdata, "%i\t %i \t %f\t%i\n\r", i * 3, irAvg[i],
                    sonarAvg[i], objectCounter);
        }

        else
        {
            sprintf(puTTYdata, "%i\t %i \t %f\t\n\r", i * 3, irAvg[i],
                    sonarAvg[i]);
        }
        uart_sendStr(puTTYdata);

    }

    double smallestWidth = 1000000;
    double objectWidth[60];
    double radialSize[60];
    int objectMidpoint[60];
    double finalObjectDistance = 0;

    for (i = 0; i <= objectCounter; i++)
    {
        if (objectStartAngle[i] >= 0 && objectStartAngle[i] <= 180
                && objectEndAngle[i] >= 0 && objectEndAngle[i] <= 180)
        {
            radialSize[i] = objectEndAngle[i] - objectStartAngle[i];

            objectMidpoint[i] = (objectStartAngle[i] + (radialSize[i] / 2.0));
        }
        else
        {
            objectMidpoint[i] = objectStartAngle[i];
            radialSize[i] = 10000;
        }
        if (objectMidpoint[i] % 3 == 1)
        {
            objectMidpoint[i] -= 1;
        }
        else if (objectMidpoint[i] % 3 == 2)
        {
            objectMidpoint[i] += 1;
        }

        objectWidth[i] = (6.2830 * sonarAvg[objectMidpoint[i] / 3]
                * radialSize[i]) / 360.0;

        char objectWidthToString[10];
        sprintf(objectWidthToString, "\r\n%f", objectWidth[i]);

        uart_sendStr(objectWidthToString);
        if (objectWidth[i] < smallestWidth && objectWidth[i] > 0.005)
        {
            smallestWidth = objectWidth[i];
            targetAngle = objectMidpoint[i];
            finalObjectDistance = sonarAvg[objectMidpoint[i] / 3];
        }
    }
    cyBOT_Scan(targetAngle, &getScan);

    char angleOutput[10];
    sprintf(angleOutput, "\r\nTarget Angle: %i\r\nTargetDistance:%f",
            targetAngle, finalObjectDistance);
    uart_sendStr(angleOutput);
    scan_flag = 0;
    return finalObjectDistance;

}

