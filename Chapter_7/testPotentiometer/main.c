/*
 * testPotentiometer.c
 *
 * Created: 12/30/2025 11:20:41 PM
 * Author : ASUS
 */ 

#define F_CPU 12000000UL
#include <avr/io.h>
#include <util/delay.h>
#include "USART_RS232.h"

#define	LED_PORT	PORTB
#define	LED_DDR		DDRB
#define LED0		PORTB0

static inline void initADC0(void)
{
	// Bit 7:6 = 01 -> AVCC with external capacitor at AREF pin
	// Bit 5 = 0: Right Adjust
	// Bit 3 = 0: 0000 -> ADC0
	// ADMUX = 0b01000000
	ADMUX |= (1 << REFS0);
	ADMUX &= ~(1 << ADLAR);			// Clear bit ADLAR -> Right Adjust
	
	// Bit 7 = 1: ADC enable
	// Bit 6 = 0: don't start conversion yet
	// Bit 5 = 0: disable auto trigger
	// Bit 3 = 0: disable ADC interrupt
	// Bit 2:0 = 110 -> ADC prescale = 64 -> ADC clock = 12MHz / 64 = 187.5kHz
	// Note: ADC requires an input clock frequency less than 200KHz for max
	// ADCSRA = 0b10000110
	ADCSRA |= (1 << ADEN) | (1 << ADPS2) | (1 << ADPS1);
}

int main(void)
{
	uint16_t adcValue;
	initADC0();
	initUSART();
	
	LED_DDR = (1 << LED0);
	
    /* Replace with your application code */
    while (1) 
    {
		ADCSRA |= (1 << ADSC);				// start ADC conversion
		while (ADCSRA & (1 << ADSC));		// Loop until ADC conversion completes
		adcValue = ADC;						// read ADC in
		
		// Display LED
		if (adcValue > 500)
			PORTB ^= (1 << LED0);
		
		printWord(adcValue);				// Transmit ADC Value to check
		printString("\r\n");
		_delay_ms(1000);
    }
}

