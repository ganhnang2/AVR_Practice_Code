/*
	amRadio.c
	
	Plays a simple tune, broadcasts it in the AM radio band.
	+) The waveform frequency generated on OCnx pin is defined by the following equation:
					fclk_I/O
	fOCnx = --------------------------
				2 x N x (1 + OCRnx)
	
	N: pre-scaler factor (1, 8, 64, 256, 1024)
	OCRnx: The register contains an value that is continuously compared with the counter value (TCNT0)
	
	+) NOTE: AM radio normally lives is usually measured in kiloHertz and runs from around 500 kHz to around 1600 kHz.
	+) Good values for the AM band: 0 -> carrier frequency: 750kHz
	Divide by two b/c we're toggling on or off each loop; a full cycle of the carrier takes two loops.
	12MHz / 2 * 1 * (3 + 1) = 1500kHz
 */ 



#define F_CPU	12000000UL					// Clock CPU: 12MHz
// ------- Preamble -------- //
#include <avr/io.h>                        /* Defines pins, ports, etc */
#include <util/delay.h>                     /* Functions to waste time */
#include <avr/interrupt.h>
#include "scale16.h"

#define COUNTER_VALUE   3              /* determines carrier frequency */
#define ANTENNA                 PD5                            /* OC0B */
#define ANTENNA_PORT            PORTD
#define ANTENNA_PIN             PIND
#define ANTENNA_DDR             DDRD

static inline void initTimer0(void) {
	TCCR0A |= (1 << WGM01);                                  /* CTC mode */
	TCCR0A |= (1 << COM0B0);            /* Toggles pin each time through */
	TCCR0B |= (1 << CS00);              /* Clock at CPU frequency, ~12MHz */
	OCR0B = COUNTER_VALUE;                          /* carrier frequency, ~1500kHz */
}

static inline void initTimer1(void) {
	TCCR1B |= (1 << WGM12);                                  /* CTC mode */
	TCCR1B |= (1 << CS11);            /* Clock at CPU/8 frequency, ~1.5MHz */
	TIMSK1 |= (1 << OCIE1A);          /* enable output compare interrupt */
}

ISR(TIMER1_COMPA_vect) {                 /* ISR for audio-rate Timer 1 */
	ANTENNA_DDR ^= (1 << ANTENNA);          /* toggle carrier on and off */
}

static inline void transmitBeep(uint16_t pitch, uint16_t duration) {
	OCR1A = pitch;                               /* set pitch for timer1 */
	sei();                                         /* turn on interrupts */
	do {
		_delay_ms(1);                            /* delay for pitch cycles */
		duration--;
	} while (duration > 0);
	cli();                  /* and disable ISR so that it stops toggling */
	ANTENNA_DDR |= (1 << ANTENNA);               /* back on full carrier */
}

int main(void) {
	// -------- Inits --------- //
	initTimer0();
	initTimer1();

	// ------ Event loop ------ //
	while (1) 
	{
		transmitBeep(E3, 200);
		_delay_ms(100);
		transmitBeep(E3, 200);
		_delay_ms(200);
		transmitBeep(E3, 200);

		_delay_ms(2500);

	}                                                  /* End event loop */
		return 0;                            /* This line is never reached */
}


