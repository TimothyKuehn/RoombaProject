/*
 * Scanner.h
 *
 *  Created on: Apr 28, 2023
 *      Author: gboes
 */

#ifndef SCANNER_H_
#define SCANNER_H_

extern volatile int rawVal[];

void scannerIR(void);
double scannerPING(int angle);



#endif /* SCANNER_H_ */
