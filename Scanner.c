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
void scannerIR() //Use IR to scan 180 degrees, this data is used for object detection
{

    uint16_t last = 90;
    last = servo_move(0,last);//Start at 0
	
    int i;
    for (i = 0; i < 181; i+=1)
    {

        last = servo_move(i,last);
        int avg = 0;
        int j;
        for(j = 0; j < 10; ++j){
            avg += adc_read();
        }
        rawVal[i] = (avg/10); //Avg of 10 IR values
		
        timer_waitMillis(45); //Wait 45ms between movement and scan
    }
}


double scannerPING(int angle) //Gets ping distance of a detected objects at the object's angle
{
    uint16_t last = 90;
    last = servo_move(angle,last);
    timer_waitMillis(1000);
    int avg = 0; 

              avg = (avg + ping_getDistance()); // Previously was averaging ping - but it can give bad results at times
       
          return avg;
    }


