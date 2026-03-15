/*
	Hien thi cac mau hieu ung LED khac nhau
 */ 

#define F_CPU 8000000UL			// Xung CPU 8MHz
#include <avr/io.h>
#include <util/delay.h>
#include <avr/power.h>
#include <stdlib.h>

#define DELAY_TIME 50

int main(void)
{
	// DDRD: OUTPUT
	DDRD = 0xFF;
	// Set PORTD bit 0
	PORTD = 0x00;
	
	// Tao ra seed ngau nhien
	srand(100);
	
	// Index LED
	uint8_t i = 0;
	
	uint8_t ledRandom;
	uint8_t reception;
	// Loop event
    while (1) 
    {
		/* LED 0 -> 7 (Go Left) */
		
		// Set bit 
		for (i = 0; i < 8; i++)
		{
			PORTD |= (1 << i);
			_delay_ms(DELAY_TIME);
		}
		
		// Clear bit
		for (i = 0; i < 8; i++)
		{
			PORTD &= ~(1 << i);
			_delay_ms(DELAY_TIME);
		}
		_delay_ms(2 * DELAY_TIME);
		
		/* LED 7 -> 0 (Go Right) */
		
		// Set bit
		for (i = 8; i > 0; i--)
		{
			PORTD |= 1 << (i - 1);
			_delay_ms(DELAY_TIME);
		}
		
		// Clear bit
		for (i = 8; i > 0; i--)
		{
			PORTD &= ~(1 << (i - 1));
			_delay_ms(DELAY_TIME);
		}
		_delay_ms(2 * DELAY_TIME);
		
		// Toggle random bit 
		for (reception = 0; reception < 10; reception++)
		{
			ledRandom = rand() % 8;				// Random LED
			PORTD ^= 1 << ledRandom;			// Toggle random bit
			_delay_ms(DELAY_TIME);
		}
		
		// Reset cho lan lap sau
		PORTD = 0;
		_delay_ms(2 * DELAY_TIME);
    }
	
	return 0;		
}

