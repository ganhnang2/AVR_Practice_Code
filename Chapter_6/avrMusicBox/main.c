/*
	Music Box Input Demo
 */ 

#define F_CPU 8000000UL						// Clock CPU: 8MHz
#include <avr/io.h>
#include <util/delay.h>
#include "scale16.h"

/*	Define Speaker pin	*/
#define SPEAKER_DDR		DDRD
#define SPEAKER_PORT	PORTD
#define SPEAKER_PIN		PIND
#define SPEAKER			PORTD7

/*	Define Button pin	*/
#define BUTTON_DDR		DDRB
#define BUTTON_PORT		PORTB
#define BUTTON_PIN		PINB
#define BUTTON			PORTB0

#define SONG_LENGTH			(sizeof(song)/ sizeof(uint16_t))

/*	Function for organ	*/
void playNote(uint16_t period, uint16_t duration) {
	uint16_t elapsed;
	uint16_t i;
	for (elapsed = 0; elapsed < duration; elapsed += period) {
		/* For loop with variable delay selects the pitch */
		for (i = 0; i < period; i++) {
			_delay_us(1);
		}
		SPEAKER_PORT ^= (1 << SPEAKER);
	}
}

void rest(uint16_t duration) {
	do {
		_delay_us(1);
	} while (--duration);
}

int main(void)
{
    const uint16_t song[] = {
	    E6, E6, E6, C6, E6, G6, G5,
	    C6, G5, E5, A5, B5, Ax5, A5,
	    G5, E6, G6, A6, F6, G6, E6, C6, D6, B5,
	    C6, G5, E5, A5, B5, Ax5, A5,
	    G5, E6, G6, A6, F6, G6, E6, C6, D6, B5,
    };
	
	/*	Bat dau tu note cuoi cung boi vi khi an nut thi se tu dong nhay sang note dau tien
		va dieu nay giup bai hat bat dau tu dau		*/
	uint8_t whichNote = SONG_LENGTH - 1;
	uint8_t wasButtonPressed = 0;
	
	
	//----------------------Init----------------------------//
	SPEAKER_DDR |= (1 << SPEAKER);			// Set SPEAKER pin output
	BUTTON_DDR &= ~(1 << BUTTON);			// Set BUTTON pin input
	BUTTON_PORT |= (1 << BUTTON);			// Enable Rpullup
	
	
	//----------------------Event loop----------------------//	
    while (1) 
    {
		// If button is pressed now, ... 
		if (!(BUTTON_PIN & (1 << BUTTON)))
		{
			if (!wasButtonPressed)			// The last state isn't pressed
			{
				whichNote++;				// increment variable pointing to note
				
				//	Reset song
				if (whichNote == SONG_LENGTH)
					whichNote = 0;
				
				wasButtonPressed = 1;
			}
			
			// Play note in 1600uS
			playNote(song[whichNote], 1600);
		}
		else
			wasButtonPressed = 0;
    }
	
	return 0;
}

