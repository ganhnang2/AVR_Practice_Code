/*
	waveformGeneratorByCTC.c
	TCT mode allows greater control of the compare match output frequency.
	In this sketch, i use external crystal 12MHz as internal clock and Timer/Counter0
	
	+) For generating a waveform output in CTC mode, the OC0A output can be set 
	to toggle its logical level on each compare match (COM0A1:0 = 1)
	-> As soon as TCNT0 becomes equal to the OCR0, a compare match occurs, and 
	then the timer will get cleared (TCNT0 = 0) and the OCF0 flag will get set.
	NOTE: OCnx pin must be configured as output by DDR register
	
	+) The waveform frequency generated on OCnx pin is defined by the following equation:
					fclk_I/O
	fOCnx = --------------------------
				2 x N x (1 + OCRnx)
			
	N: pre-scaler factor (1, 8, 64, 1024)
	OCRnx: The register contains an value that is continuously compared with the counter value (TCNT0)
 */ 

#define F_CPU	12000000UL
#include <avr/io.h>

int main(void)
{
	/*------------------Init----------------*/
    DDRD |= (1 << PORTD6);						// set OC0A as output
	
	// COM0A1:0 = 1 -> toggle OC0A on compare match 
	// WGM02:0 = 2  -> CTC mode
	TCCR0A |= (1 << COM0A0) | (1 << WGM01);		
	TCCR0B = (1 << CS02) | (1 << CS00);		// internal clock: 12MHz, pre-scaler: 1024
	OCR0A = 205;							// value that is continuously compared with the counter value (TCNT0)
	
	/*
		The waveform frequency on OC0A pin: 
						12MHz
		fOC0A = -------------------------  = 28.44Hz -> time cycle ~= 35.16(ms)
					2 * 1024 * (1 + 205)
					
		Wire up a LED on OCOA pin and it will blink LED once every 17.58 ms
	*/			
	
	/*----------Infinitive Loop--------------*/
    while (1) ;
}

