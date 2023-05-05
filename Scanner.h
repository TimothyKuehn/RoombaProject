/*
 * Scanner.h
 *
 *  Created on: Apr 28, 2023
 *      Author: gboes
 */

#include "adc.h"
#include "servo.h"
#include "Timer.h"
#include "lcd.h"
#include "ping.h"


#ifndef SCANNER_H_
#define SCANNER_H_

extern float rawVal[];

void scannerIR();
double scannerPING(int angle);



#endif /* SCANNER_H_ */
