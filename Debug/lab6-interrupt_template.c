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
#include <stdbool.h>
#include "driverlib/interrupt.h"

// Uncomment or add any include directives that you want to use
// #include "open_interface.h"
// #include "movement.h"
// #include "button.h"

// Your code can use the global variables defined in uart-interrupt.c
// They are declared with the extern qualifier in uart-interrupt.h, which makes the variables visible to this file.

#warning "Possible unimplemented functions"
#define REPLACEME 0


int main(void) {
	timer_init(); // Must be called before lcd_init(), which uses timer functions
	lcd_init();
	uart_interrupt_init();
  // cyBOT_init_Scan();

	// OPTIONAL
	//assign a value to command_byte if you want to know whether that ASCII code is received
	//note that command_byte is global shared variable read by the ISR
	//for example, try using a tab character as a command from PuTTY

	while(1)
	{
	    if(command_byte == 'g')
	                      {
	                       cyBOT_init_Scan(0b0011);
	                       cyBOT_Scan_t getScan;

	                         int i = 0;
	                         for(i; i<180; i+=3)
	                         {
	                          cyBOT_Scan(i, &getScan);

	                         }
	                        }
	    if(command_byte == 's')
	                                  {
	                                  command_flag = 0;
	                                  break;
	                                  }
	}

}
