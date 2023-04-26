/*
 * adc.h
 *
 *  Created on: Mar 28, 2023
 *      Author: jtyost
 */

#ifndef ADC_H_
#define ADC_H_

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <inc/tm4c123gh6pm.h>
#include "lcd.h"
#include "Timer.h"

void adc_init(void);

uint16_t adc_read(void);

uint16_t adc_averageIR(int samples);

#endif /* ADC_H_ */
