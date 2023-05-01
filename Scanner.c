/**
 * scanner.c
 * Author grantiepoo
 */

#include "adc.h"
#include "servo.h"
#include "ping.h"


volatile int rawVal[180];
volatile uint16_t last = 90;
void scannerIR(void)
{
    last = servo_move(0,last);
    int i;
    for (i = 0; i < 180; i+=1)
    {
        last = servo_move(i,last);
        int avg;
        int j;
        for(j = 0; j < 10; ++j){
            avg = avg + read();
        }
        rawVal[i] = (avg) / 10;
    }
}


double scannerPING(int angle)
{
    last = servo_move(angle,last);
    int avg;
    int j;
    for(j = 0; j < 10; ++j){
        avg = avg + ping_getDistance();
    }
    return avg / 10;
}




