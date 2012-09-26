#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/atomic.h>
#include "timer.h"
							
/************************************************************************
 *	INITIALIZE TIMER
 */
void Timer_Init(void)
{
	/*
    TIMER0 - CTC MODE
    Clock: 16Mhz
    TOP = 0xFF
    PS: 64
    Interrupt Frequency: 8 KHz
    */

    TCCR0 |= (1<<WGM01) | (1<<CS01) | (1<<CS00);   /* CTC, PS: 64 */
    TIMSK |= (1<<OCIE0);                           /* Enable Interrupt */
    OCR0 = 29; 
}
