#include <avr/io.h>
#include <util/delay.h>
#include <math.h>
#include "comp.h"


/************************************************************************
 *	GLOBAL VARIABLES
 */
extern volatile float *x;									/* Input signal Buffer */
extern volatile float *X;									/* DFT Output */

/************************************************************************
 *	INITIALIZE FFT
 */

void DFT_Init	( uint16_t N )
{
	x = (float*) malloc( sizeof(float) * N );
	X = (float*) malloc( sizeof(float) * N );
}

/************************************************************************
 *	DFT
 */
void DFT ( uint16_t Np )
{
	float Re = 0, Im = 0;
	
	for( uint16_t k = 0; k < Np; k++ ){
		
		Re = 0; Im = 0;										/* initialize real and imaginary parts to zero */
		
		for( uint16_t i = 0; i < Np; i++ )					/* Real Part */
			Re += ( x[i] * cos ( TWO_PI * k * i / Np ) );
		
		for( uint16_t i = 0; i < Np; i++ )					/* Imaginary Part */
			Im += ( x[i] * sin ( TWO_PI * k * i / Np ) );
		
		X[k] = sqrt( ( Re * Re ) + ( Im * Im ) );		
	}
}

