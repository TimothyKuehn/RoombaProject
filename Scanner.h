/*
 * Scanner.h
 *
 *  Created on: Apr 28, 2023
 *      Author: gboes
 */

#ifndef SCANNER_H_
#define SCANNER_H_

extern volatile int rawVal[];
extern volatile uint16_t last;
void scannerIR(void);
double scannerPING(int angle);



#endif /* SCANNER_H_ */
