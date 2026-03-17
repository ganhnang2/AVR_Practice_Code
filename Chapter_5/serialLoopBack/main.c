/*

 */ 
#include <avr/io.h>
#include "USART.h"
#include <util/delay.h>

int main(void)
{
	char serialCharacter;
	
	//------------Init----------------//
	// Control LED
	// LED 0:5 = Pin B 0:5
	// LED 6:7 = Pin D 6:7
    DDRB = 0x3F;		// Configure pin B0:5 output
    DDRD = 0xC0;		// Configure pin D6:7 output
	
	initUSART();
	printString("Hello World\r\n");
	
	//------------Loop event----------------//
    while (1) 
    {
		serialCharacter = receiveByte();
		transmitByte(serialCharacter);
		
		// Reset PORTB, D
		PORTB = 0x00;
		PORTD = 0x00;
		
		// Control LED with UDR0
		// Bit 0 -> 5 = PORTD0:5
		// Bit 6 -> 7 = PORTD6:7
		PORTB = (serialCharacter & ~(1 << PORTB6 | 1 << PORTB7));
		PORTD = (serialCharacter & (1 << PORTD6 | 1 << PORTD7));
    }
}

