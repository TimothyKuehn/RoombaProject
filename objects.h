/*
 * objects.h
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

#ifndef OBJECTS_H_
#define OBJECTS_H_

typedef struct{
    float distance;
    int angle;
    float width;
}Objects;



void identifyObjects(float inputDistances[]);
void sendObjectArr(Objects *arr, int numObjects);



#endif /* OBJECTS_H_ */
