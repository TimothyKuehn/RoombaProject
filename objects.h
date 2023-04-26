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
#include "movement.h"

#ifndef OBJECTS_H_
#define OBJECTS_H_



void printObjectArr(struct Objects *arr,int numObjects);
void printToPutty(float distance, int i);





#endif /* OBJECTS_H_ */
