/*
 * DFT.c
 *
 * Musical Fountain - Techtatva 2012
 *
 * Created: 26-09-2012 PM 10:21:37
 *  Author: Ayush Sinha
 */ 

#define F_CPU	16000000
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "uart.h"
#include "adc.h"
#include "timer.h"
#include "comp.h"

/************************************************************************
 *	DEFINES
 */
#define THRES		70
#define DC_THRES	3800
#define DFT_TIME	(50 * 8)							/* 8 counts = 1 ms */
#define TERMINAL	1

/************************************************************************
 *	ENUMERATIONS
 */
typedef enum
{
	WAIT = 0,
	READ_ADC,
	FIND_DFT
	
} State;

/************************************************************************
 *	GLOBAL VARIABLES
 */
volatile uint16_t ticker = 0;;

volatile float *x;
volatile float *X;

uint16_t N = 8;
volatile State state = WAIT;
volatile uint16_t index = 0;

/************************************************************************
 *	MAIN
 */
int main(void)
{
    
	DDRC = 0xFF;										/* Initialize Output Port */
	DDRA = 0x00;										/* Initialize Input Port */
	PORTC = 0xFF;

#if (TERMINAL == 1)
	UART_Init();										/* UART */
#endif

	ADC_Init();											/* ADC */
	Timer_Init();										/* Timer */
	DFT_Init( N );										/* DFT */
	
	sei();												/* Enable Interrupts */
	
	while(1)
	{		
		
    }
	
	return 0;
}

/************************************************************************
 *	TIMER ISR
 */
ISR( TIMER0_COMP_vect )
{	
	ticker++;												/* Increment Ticker */
	
	if( ( ticker == DFT_TIME ) && ( state == WAIT ) ){		/* DFT Computation Interval */
		state = READ_ADC;									/* Begin reading if DFT wait time is up */						
	}
	
	if( state == READ_ADC ){								/* ADC State */
	
		if( index < N ){
			x[index++] = (float) ( ADC_read(1) );			/* Read ADC channe 1 and Fill Buffer */
		}
		else{
			index = 0;										/* Reset index if buffer full */
			state = FIND_DFT;								/* change state for calculating DFT */
		}	
	}
	
	if( state == FIND_DFT ){								/* DFT State */
		
		DFT( 8 );											/* Calculate DFT */

#if ( TERMINAL == 1 )		
		for( uint16_t i = 0; i < N; i++ ){					/* Debug */
			
			UART_TxInt( (uint16_t) X[i] );
			UART_TxChar(' ');
		}
#endif
				
		PORTC = 0x00;										/* Display on LED */
		
		if( (uint16_t) X[0] > DC_THRES )
			PORTC |= (1<<7);
		
		if( (uint16_t) X[1] > THRES )
			PORTC |= (1<<6);
		
		if( (uint16_t) X[2] > THRES )
			PORTC |= (1<<5);
			
		if( (uint16_t) X[3] > THRES )
			PORTC |= (1<<4);
			
		if( (uint16_t) X[4] > THRES )
			PORTC |= (1<<3);
			
		if( (uint16_t) X[5] > THRES )
			PORTC |= (1<<2);
			
		if( (uint16_t) X[6] > THRES )
			PORTC |= (1<<1);
			
		if( (uint16_t) X[7] > THRES )
			PORTC |= (1<<0);
			
#if	( TERMINAL == 1 )		
		UART_TxChar('\n');	
#endif
		ticker = 0;											/* Reset Timer */
		state = WAIT;										/* Change State */
	}	
}