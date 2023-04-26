/*
 * servo.h
 *
 *  Created on: Apr 11, 2023
 *      Author: jtyost
 */

#ifndef SERVO_H_
#define SERVO_H_

#include "Timer.h"
#include "lcd.h"

void servo_init(void);

uint16_t move_servo(uint16_t degrees, uint16_t last);

#endif /* SERVO_H_ */
