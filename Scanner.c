/**
 * scanner.c
 * Author grantiepoo
 */

#include "adc.h"
#include "servo.h"
#include "Timer.h"
#include "lcd.h"
#include "ping.h"


float rawVal[180];
void scannerIR()
{

    servo_init();
    uint16_t last = 90;
    last = servo_move(0,last);
    int i;
    for (i = 0; i < 180; i+=1)
    {

        last = servo_move(i,last);
        int avg = 0;
        int j;
        for(j = 0; j < 10; ++j){
            avg += adc_read();
        }
        rawVal[i] = (avg/10);
        timer_waitMillis(40);
    }
}


double scannerPING(int angle)
{
    servo_init();
    ping_init();
    uint16_t last = 90;
    last = servo_move(angle,last);
    int avg = 0;
    int j;
    for(j = 0; j < 10; ++j){

        avg = (avg + ping_getDistance()); //Likely the Problem
    }
    avg = (avg/10);
    return avg;
}
