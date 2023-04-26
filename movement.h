/*
 * movement.h
 *
 *  Created on: Feb 3, 2023
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


#ifndef MOVEMENT_H_
#define MOVEMENT_H_

void move_forward(oi_t *sensor_data, double distance_mm);

void move_backward(oi_t *sensor_data, double distance_mm);

void turn_left(oi_t *sensor, double degrees);

void turn_right(oi_t *sensor, double degrees);

void bump(oi_t *sensor_data);



#endif /* MOVEMENT_H_ */
