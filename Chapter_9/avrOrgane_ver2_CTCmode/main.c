/*
	avrOrgane_ver2_CTCmode.c
	
	+) In avrOrganeByWaveformGenerator.c, i generated square waveform automatically with toggle on compare match of CTC mode.
	However, this will have an application on 6 pins of Timer/Counter. As a result, i will write manually pulses for musical note
	on any pins by using ISR TIMER0_COMP
	+) To enable interrupt when compare matching, COM0A1:0 = 00 and OCIE0A = 1
	+) In this version, there are some differences from the last version. First, i will create musical note by output compare interrupt. 
	Second, disable interrupt and the speaker after playing a tone.
	
	Source: Nguyen Hai Nam
 */ 

#define F_CPU	12000000UL					// Clock CPU: 12MHz
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "pitches.h"

/*	Define device pin	*/
#define SPEAKER_DDR				DDRD
#define SPEAKER_PORT			PORTD
#define SPEAKER_PIN				PIND
#define SPEAKER					PORTD7

/*	Counter for toggling pulses	*/
volatile int16_t toggleCount = 0;

/*	Function for initiating CTC mode in Timer0	*/
static inline void initTimer0()
{	
	SPEAKER_DDR |= (1 << SPEAKER);		// set as output
	
	// WGM02:00 = 2 -> CTC mode
	// COM0A1:0 = 00 -> control manually
	TCCR0A |= (1 << WGM01);
	
	sei();			// enable global interrupt
}

/*	Function playTone()	with frequency (Hz) and duration (ms)	*/
void playTone(uint16_t frequency, uint16_t duration)
{	
	/*	Choose fix prescaler for note */
	uint32_t ocr;						// calculate value in Output compare Register (OCR0A)
	uint8_t prescalerBits;				// store prescaler bits										
	
	SPEAKER_DDR |= (1 << SPEAKER);
	
	ocr = (F_CPU / frequency / 2 / 1) - 1;						// calculate value in OCR0A with prescaler = 1
	prescalerBits = (1 << CS00);								// setup prescaler factor in TCCR0B
	if (ocr > 255)
	{
		ocr = (F_CPU / frequency / 2 / 8) - 1;					// calculate value in OCR0A with prescaler = 8
		prescalerBits = (1 << CS01);
		
		if (ocr > 255)
		{
			ocr = (F_CPU / frequency / 2 / 64) - 1;				// calculate value in OCR0A with prescaler = 64
			prescalerBits = (1 << CS00) | (1 << CS01);
			
			if (ocr > 255)
			{
				ocr = (F_CPU / frequency / 2 / 256) - 1;		// calculate value in OCR0A with prescaler = 256
				prescalerBits = (1 << CS02);
				
				if (ocr > 255)
				{
					ocr = (F_CPU / frequency / 2 / 1024) - 1;	// calculate value in OCR0A with prescaler = 1024
					prescalerBits = (1 << CS02) | (1 << CS00);
				}
			}
		}
	}

	TCCR0B = (TCCR0B & 0b11110000) | prescalerBits;
	OCR0A = ocr;				// value that is continuously compared with the counter value (TCNT0)
	TIMSK0 |= (1 << OCIE0A);	// enable interrupt	
	
	while (duration)
	{
		_delay_ms(1);
		--duration;
	}
	
	SPEAKER_DDR &= ~(1 << SPEAKER);		// turn the speaker off
	TIFR0 |= (1 << OCF0A);				// NOTE: clear interrupt flag to prevent running ISR
	TIMSK0 &= ~(1 << OCIE0A);			// disable interrupt
}


/*	Interrupt is excuted when matching compare in Timer/Counter0	*/
ISR(TIMER0_COMPA_vect)
{
	SPEAKER_PORT ^= (1 << SPEAKER);		// toggle the state -> Pulse of musical note		
}

int main(void)
{
    /*------------Inits--------------*/
	initTimer0();
    
    /*----------Event loops--------*/
    while (1)
    {
	    /*	Playing a level up sound	*/
	    playTone(NOTE_E4, 200);
	    playTone(NOTE_G4, 200);
	    playTone(NOTE_E5, 200);
	    playTone(NOTE_C5, 200);
	    playTone(NOTE_D5, 200);
	    playTone(NOTE_G5, 200);
	    
	    _delay_ms(1000);
	    _delay_ms(1000);
	    
	    /*	Play a Wah-Wah-Wah-Wah sound	*/
	    playTone(NOTE_DS5, 300);
	    playTone(NOTE_D5, 300);
	    playTone(NOTE_CS5, 300);
		
	    for (uint8_t i = 0; i < 10; i++)
	    {
		    for (int8_t pitch = -10; pitch < 10; pitch++)
		    {
			    playTone(NOTE_C5 + pitch, 5);
		    }
	    }
	    
	    _delay_ms(1000);
	    _delay_ms(1000);
    }
}

