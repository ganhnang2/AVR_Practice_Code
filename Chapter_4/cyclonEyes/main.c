/*
	Bat tung LED mot theo chieu tu tren xuong duoi, va tu duoi len tren
 */ 

#define F_CPU 8000000UL			// ??t xung CPU 8MHz
#include <avr/io.h>
#include <util/delay.h>

#define DELAY_TIME 50

int main(void)
{
	// DDRD: OUTPUT
	DDRD = 0xFF;
	// Set PORTD bit 0
	PORTD = 0x00;
	
	// Index LED
    uint8_t index = 0;
	
	// Loop event
    while (1) 
    {
		// Thuc hien set bit LED lien tiep nhau tu 0 -> 7
		while (index < 8)
		{
			// Set bit index lên 1
			PORTD |= (1 << index);
			_delay_ms(DELAY_TIME);
		}
		
		// Thuc hien clear bit LED lien tiep nhau tu 7 -> 0
		while (index)
		{
			PORTD &= ~(1 << (index - 1));
			_delay_ms(DELAY_TIME);
			index--;
		}
		
		_delay_ms(2 * DELAY_TIME);
    }
}

