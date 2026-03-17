/*
	timerInputCapture.c
	+) The input capture function in Timer/Counter1 is used in many applications such as:
		- Pulse width measurement
		- Period measurement
		- Capturing the time of an event
	
	+) In the sketch, i use Timer/Counter1 to measure duty cycle on ICP pin.
	After that, display time for duty cycle on RealTerm or LED to check
	NOTE: Crystal Oscillator 12MHz is used for frequency system
	
	+) Explain the second line of the equation: 
	numberOfClocks = (uint32_t)(ending_edge) + ((uint32_t)(overflow_count * 65536)) - (uint32_t)starting_edge;
	
	First:
		start			end
		   \			  \
			\			   \
			---------		---------
			|		|		|		|
	---------		---------		---------
			<--------------->
				TCNT1 
	
	TCNT1 (First) = 50000 -> When ISR excutes, ICR1 = TCNT1
	numberOfClocks = 50000 (end) + (0 * 65536) - 0 (start) = 50000
	start = end = 50000
	
	Second:
					  start				end
		   				  \				   \
						   \				\
			---------		---------		---------
			|		|		|		|		|		|
	---------		---------		---------		---------
					
			<--------------------><---------->
				Overflow			TCNT1
					
	TCNT1 (Second) = 34464 -> When ISR excutes, ICR1 = TCNT1
	numberOfClocks = 34464 (end) + (1 * 65536) - 50000 = 50000
	
	So on ...
 */ 

#define F_CPU	12000000UL				// Clock CPU: 12MHz
#include <avr/io.h>
#include <avr/interrupt.h>
#include "USART_RS232.h"

#define LED_PORT	PORTD
#define LED_DDR		DDRD

volatile uint16_t overflow_count = 0;							// counter for timer 1 overflow
volatile uint16_t starting_edge = 0, ending_edge = 0;			// storage for timers
volatile uint32_t numberOfClocks;								// storage for actual clock counts in the pulse

/*	Timer overflow interrupt	*/
ISR(TIMER1_OVF_vect)
{
	overflow_count++;				// count the amount of roll overs	
}

/*	Input capture interrupt	*/
ISR(TIMER1_CAPT_vect)
{
	ending_edge =  ICR1;						// store the timer for current capture
	
	// calculate the total clocks for a duty cycle in ICP pin
	// overflow_count * 65536 = overflow_count << 16
	numberOfClocks = (uint32_t)(ending_edge) + ((uint32_t)(overflow_count * 65536)) - (uint32_t)starting_edge;
	
	// time of a duty cycle in ICP pin (ms), display on LED to check
	// Timer/Counter1 has 187.5 clock / 1ms -> Total time (ms) = numberOfClock / 187.5
	// LED_PORT = (numberOfClocks / 187.5);
	printWord(numberOfClocks / 187.5);
	printString("\r\n");
	
	overflow_count = 0;						// reset counter for overflows
	starting_edge = ending_edge;			// update start for next capture
}

int main(void)
{
    /*----------------Inits------------------*/
	// LED_DDR = 0xFF;
	initUSART();
	TCCR1A = 0x00;										// normal mode
	
	// no noise canceler
	// trigger capture: rising edge 
	// prescaler: 64 -> clock = 12MHz/64 = 187.5kHz (i.e: 1ms has 187.5 cycles)
	TCCR1B |= (1 << ICES1) | (1 << CS11) | (1 << CS10);	
	TIMSK1 |= (1 << ICIE1) | (1 << TOIE1);				// enable input capture and timer overflow interrupt
	sei();												// enable global interrupt
	
    while (1);
}

