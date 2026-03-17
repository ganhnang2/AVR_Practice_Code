/*
	timerDelay.c
	
	+) For example, a program is to toggle the state of an LED every 2 seconds.	
	+) In the sketch, Timer 0 is used to create timeout 2s for blinking LED
	-> Timer 0 is more often used to create a time base or tick for the program
	NOTE: Crystal Oscillator 12MHz is used for frequency system
	+) If the tick occurs every 19.2 ms, we would want the counter (time_count) to count up to 105
	before toggling the LED (105 * 19.2 ms ~= 2s).
	
	+) Process:
	1. A number is selected and loaded into the timer (TCNT0)
	2. The timer counts from this number up to 255 and roll overs. 
	3. Whenever it rolls over, it creates an interrupt. ISR reload the same number into the timer
	4. The cycle then repeats, with the counter counting up from the number that was loaded to 255, 
	and rolls over, creating another interrupt.
	So on ...
 */ 

#define F_CPU	12000000UL					// Clock CPU: 12MHz
#include <avr/io.h>
#include <avr/interrupt.h>

#define LED_PORT	PORTB
#define LED_DDR		DDRB
#define LED0		PORTB0

volatile uint16_t time_count;

/*	+) Value to load in TCNT0:
	TCNT0 = 256 - 225 (225 clock ticks to overflow the timer) = 31
	+) The time possible using the counter alone:
	85.33us * 225 = 19200us = 19.2ms
	-> ISR will be excuted once every 19.2ms	
	-> time_count = 2s/ 19.2ms ~= 105	*/	
ISR(TIMER0_OVF_vect)
{
	TCNT0 = 31;								// set for 19.2ms timeout
	++time_count;							// increment time_count
	
	if (time_count >= 105)
	{
		LED_PORT ^= (1 << LED0);			// blink LED
		time_count = 0;						// reset time_count for next 2s
	}
}

int main(void)
{
	/*--------------Inits-------------*/
	LED_DDR |= (1 << LED0);					// mode: output
	
	time_count = 0;
	TCNT0 = 0x00;							// initialize timer/counter register 
	TCCR0A = 0x00;							// normal mode
    TCCR0B = (1 << CS02) | (1 << CS00);		// internal clock: 12MHz, pre-scaler: 1024 -> clock: 12MHz/ 1024 -> time cycle = 85.33us
	TIMSK0 = (1 << TOIE0);					// enable Timer 0 overflow interrupt
	sei();									// enable global interrupt
	
	/*---------Infinitive loop----------*/
    while (1) ;
}

