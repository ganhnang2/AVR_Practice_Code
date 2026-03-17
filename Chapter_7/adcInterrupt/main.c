/*
	adcInterrupt.c

	Demonstrate for a limit detector system based on the analog input voltage to ADC channel 3.
	In the sketch, the ADC is usually used in an interrupt-driven mode
 */ 

#define F_CPU 12000000UL
#include <avr/io.h>
#include <avr/interrupt.h>

#define LED_PORT		PORTB
#define LED_DDR			DDRB
#define LED_GREEN		0b00000100
#define LED_BLUE		0b00000010
#define LED_RED			0b00000001

/*	ADC interrupt	*/
ISR(ADC_vect)
{
	uint16_t adcData;
	adcData = ADC;							// store data when ADC conversion is complete
	if (adcData > (3 * 1023 / 5))			// > 3V
		LED_PORT = LED_RED;
	else if (adcData < (2 * 1023 / 5))		// < 2V
		LED_PORT = LED_BLUE;
	else
		LED_PORT = LED_GREEN;
		
	ADCSRA |= (1 << ADSC);					// start new conversion
}

int main(void)
{
    /*----------------Inits--------------*/
	// AVCC with external capacitor at AREF pin, channel: ADC3
	ADMUX |= (1 << REFS0) | (1 << MUX1) | (1 << MUX0);	
	ADMUX &= ~(1 << ADLAR);	
	
	// ADC enable, start first conversion, prescaler: 64 -> ADC clock: 12MHz/64 = 187.5kHz
	ADCSRA |= (1 << ADEN) | (1 << ADPS2) | (1 << ADPS1) | (1 << ADSC) | (1 << ADIE);
	
	// mode: output
	LED_DDR |= (1 << PORTB0) | (1 << PORTB1) | (1 << PORTB2);
	sei();					// enable global interrupt
    while (1);
}

