/**
 * scanner.c
 *   @Author grantiepoo
 */

#include "adc.h"
#include "servo.h"
#include "Timer.h"
#include "lcd.h"
#include "ping.h"

// Define an array to store the raw sensor values
float rawVal[180];

// This function uses a servo motor to move an infrared sensor to different angles and
// records the raw sensor values for each angle
void scannerIR()
{
    // Start at a default angle of 90 degrees
    uint16_t last = 90;
    last = servo_move(0,last);
    
    // Move the servo to each angle from 0 to 180 degrees and record the raw sensor value
    for (int i = 0; i < 181; i+=1)
    {
        // Move the servo to the current angle
        last = servo_move(i,last);
        
        // Take the average of 10 sensor readings
        int avg = 0;
        for(int j = 0; j < 10; ++j){
            avg += adc_read();
        }
        rawVal[i] = (avg/10);
        
        // Wait for a short amount of time before moving to the next angle
        timer_waitMillis(45);
    }
}

// This function moves a servo motor to a specified angle and returns the distance
// measured by a PING sensor at that angle
double scannerPING(int angle)
{
    // Start at a default angle of 90 degrees
    uint16_t last = 90;
    last = servo_move(angle,last);
    
    // Wait for the servo to settle before taking a sensor reading
    timer_waitMillis(1000);
    
    // Take a single PING sensor reading and return the distance
    int avg = ping_getDistance();
    return avg;
}

// This function moves a servo motor to a specified angle and returns the distance
// measured by an infrared sensor at that angle
double IRDist(int angle){
    // Start at a default angle of 90 degrees
    uint16_t last = 90;
    last = servo_move(angle,last);
    
    // Wait for the servo to settle before taking sensor readings
    timer_waitMillis(20);
    
    // Take the average of 10 sensor readings
    int avg = 0;
    for(int j = 0; j < 10; ++j){
        avg = (avg + adc_distance());
    }
    avg = (avg/10);
    
    // Return the distance
    return avg;
}

