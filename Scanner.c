/**
 * scanner.c
 * Author grantiepoo
 */

#include "adc.h"
#include "servo.h"
#include "ping.h"


volatile int rawVal[60];
void scannerIR(void)
{
    int val1, val2;

    int i;
    for (i = 0;i < 181; i+=3)
    {
        val1 = read();
        val2 = read();
        rawVal[i/3] = (val1 + val2) / 2;
        servo_move(i);
    }

}


double scannerPING(int angle)
{
    double val0, val1, val2;
    double distance;
    servo_move(angle);
    val0 = ping_getDistance();
    val1 = ping_getDistance();
    val2 = ping_getDistance();

    distance = (val0 + val1 + val2) / 3;
    return distance;
}
