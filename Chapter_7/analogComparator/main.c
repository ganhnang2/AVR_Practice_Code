/*
	analogComparator.c
	
	+) ATmega328P based on AVR has two pins for analog voltage compare i.e. AIN0 and AIN1. 
	AIN0 is the positive terminal whereas AIN1 is the negative terminal.
	+) It is possible to use ADC channels (PA0 to PA7) as a negative terminal (AIN1) of a comparator. 
	In this condition, AIN1 will not be considered as a negative input to the comparator. ADC multiplexer 
	is used to select the ADC channel to be connected. ADC must be switched off to use this feature.
	+) Analog Compare Interrupt Mode Select Bit Definitions:
	ACIS1:0 = 00 -> Comparator interrupt on ACO toggle
	AICS1:0 = 01 -> Reversed
	AICS1:0 = 10 -> Comparator interrupt on ACO falling edge (AIN1 becomes greater than AIN0)
	AICS1:0 = 11 -> Comparator interrupt on ACO rising edge (AIN0 becomes greater than AIN1)
	
	+) In the sketch, we will use analog comparator functioning to detect whenever the battery 
	voltage becomes dangerous low.
	-> The analog comparator is set to detect when the voltage from the battery’s voltage divider drops
	below the 2.2 V provided by the reference divider.
	-> When the battery voltage becomes too low, the LED is lighted.
 */ 

#define F_CPU	12000000UL				// Clock CPU: 12MHz
#include <avr/io.h>
#include <avr/interrupt.h>

volatile uint16_t blinkCount = 0;

#define LED_DDR		DDRB
#define LED_PORT	PORTB
#define LED0		PORTB0

/*	Timer0 overflow interrupt	*/
ISR(TIMER0_OVF_vect)
{
	TCNT0 = 256 - 12;			// 12 cycle ~= 12 * 85.33 = 1.023ms
	++blinkCount;
	switch (blinkCount)
	{
		case 1900:				// Wait until time counter = 1.9s, LED on
			LED_PORT |= (1 << LED0);
			break;
		case 2000:				// Blink LED 0.1s, LED off
			LED_PORT &= ~(1 << LED0);
			blinkCount = 0;		// reset counter
			break;
		default:
			break;
	}
}

/*	Analog comparator interrupt
	NOTE:
	falling output edge means: first, AIN0 > AIN1 -> ACO = 1. After that, AIN0 < AIN1 -> ACO = 0
	(state ACO: 1 -> 0)
	-> falling ouput edge is actived -> ISR is excuted		*/
ISR(ANALOG_COMP_vect)
{
	ACSR |= (1 << ACD);		// disable analog comparator
	
	TCCR0A = 0x00;			// normal mode
	TCCR0B = (1 << CS02) | (1 << CS00);		// prescaler: 1024 -> clock: 12MHz / 1024 -> duty cycle: 85.33us
}

int main(void)
{
    /*-------------Inits-------------*/
	
	// Bit 7 (ACD): 0 -> enable anlog comparator
	// Bit 5 (ACO): 0 -> analog comparator output
	// Bit 3 (ACIE): 1 -> enable analog comaprator interrupt mask
	// Bit 2 (ACIC): 0 -> disable the input capture function in Timer/Counter1 to be trigged by the analog comparator
	// Bit 1:0 (ACIS1:0): 10 -> comparator interrupt on falling output edge (AIN0 < AIN1).
	ACSR |= (1 << ACIS1) | (1 << ACIE);
	
	// ACME = 0 -> AIN1 is applied to the negative input of the Analog Comparator
	ADCSRB = 0x00;
	
	// enable timer overflow interrupt in Timer0
	TIMSK0 |= (1 << TOIE0);
	
	LED_DDR |= (1 << LED0);
	sei();							// enable global interrupt
	
	/*----------Loop events----------*/
    while (1);
}

