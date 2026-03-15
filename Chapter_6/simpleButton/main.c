/*
	Demo of the simplest on/off button code
	Button connected to PB0
	LEDs connected to PD0..PD7
 */ 

#define F_CPU 8000000UL
#include <avr/io.h>
#include <util/delay.h>

int main(void)
{
	//------------------Inits-------------------//
	PORTB |= (1 << PORTB0);			// Enable Rpullup in PB0
	DDRD = 0xFF;					// Configure pin D output
	
	//------------------Loop event-------------------//
	while (1)
	{
		// If button is pressed (PB0= LOW), PORTD = 0b11100111
		// otherwise, PORTD = 0b00011000
		if (!(PINB & (1 << PINB0)))
		PORTD = 0b11100111;
		else
		PORTD = 0b00011000;
	}
	
	return 0;
	
}