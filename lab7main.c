/**
 * lab6-interrupt_template.c
 *
 * Template file for CprE 288 Lab 6
 *
 * @author Diane Rover, 2/15/2020
 *
 */

#include "Timer.h"
#include "lcd.h"
#include "cyBot_Scan.h"  // For scan sensors
#include "uart-interrupt.h"
#include "open_interface.h"
#include "movement.h"
#include "music.h"
#include <stdbool.h>
#include "driverlib/interrupt.h"

int main(void)
{
    timer_init(); // Must be called before lcd_init(), which uses timer functions
    lcd_init();
    uart_interrupt_init();
    oi_t *sensor_data = oi_alloc(); // do this only once at start of main()
    oi_init(sensor_data); // do this only once at start of main()
    char startupMessage[] = "STARTUP MESSAGE";
    lcd_printf(startupMessage);
    double distanceMoved;
    double angleTurned;
    while (1)
    {


        distanceMoved = 0;
        angleTurned = 0;

        if (flag_m)
        {
            distanceMoved = move_forward(sensor_data, 148.9);
            char returnString[20];
            sprintf(returnString, "D0!%lf", distanceMoved);
            uart_sendStr(returnString);
            flag_m = false;
        }
        if (flag_r)
        {
            distanceMoved = move_backward(sensor_data, 49);
            char returnString[20];
            sprintf(returnString, "D0!%lf", distanceMoved);
            uart_sendStr(returnString);
            flag_r = false;
        }

        if (flag_1)
        {
            angleTurned = turn_left(sensor_data, 89.6);
            char returnString[20];
            sprintf(returnString, "D%lf!0", angleTurned);
            uart_sendStr(returnString);
            flag_1 = false;
        }
        if (flag_2)
        {
            angleTurned = turn_left(sensor_data, 44.8);
            char returnString[20];
            sprintf(returnString, "D%lf!0", angleTurned);
            uart_sendStr(returnString);
            flag_2 = false;
        }
        if (flag_3)
        {
            angleTurned = turn_left(sensor_data, 15);
            char returnString[20];
            sprintf(returnString, "D%lf!0", angleTurned);
            uart_sendStr(returnString);
            flag_3 = false;
        }
        if (flag_s)
        {
            //SCAN SETUP
        }
        if (flag_4)
        {
            angleTurned = turn_right(sensor_data, 15);
            char returnString[20];
            sprintf(returnString, "D%lf!0", angleTurned);
            uart_sendStr(returnString);
            flag_4 = false;
        }
        if (flag_5)
        {
            angleTurned = turn_right(sensor_data, 44.8);
            char returnString[20];
            sprintf(returnString, "D%lf!0", angleTurned);
            uart_sendStr(returnString);
            flag_5 = false;
        }
        if (flag_6)
        {
            angleTurned = turn_right(sensor_data, 89.6);
            char returnString[20];
            sprintf(returnString, "D%lf!0", angleTurned);
            uart_sendStr(returnString);
            flag_6 = false;
        }
        if(flag_c)
        {
            load_songs();
            play_songs(1);
            flag_c = false;
        }

    }

}
