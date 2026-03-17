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
#define LED1		PORTB1
#define LED2		PORTB2

static inline void initADC0(void)
{
	// Bit 7:6 = 01 -> AVCC with external capacitor at AREF pin
	// Bit 5 = 1: Left Adjust
	// Bit 3 = 0: 0000 -> ADC0
	// ADMUX = 0b01000000
	ADMUX |= (1 << REFS0) | (1 << ADLAR); 
	
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
	uint8_t adcValue;
	initADC0();
	initUSART();
	
	LED_DDR = ((1 << LED0) | (1 << LED1) | (1 << LED2));
	
    /* Replace with your application code */
    while (1) 
    {
		ADCSRA |= (1 << ADSC);				// start ADC conversion
		while (ADCSRA & (1 << ADSC));		// Loop until ADC conversion completes
		adcValue = ADCH;					// only read ADCH when ADC is left-adjusted
		
		// Display LED
		if (adcValue > 125)
			PORTB ^= ((1 << LED0) | (1 << LED1) | (1 << LED2));
		
		/*	2-line is comparative to oscilloscope_ver2.py	*/
		/*	Can use to display data on RealTerm	*/
		printByte(adcValue);				// Transmit ADC Value to demonstrate oscilloscope
		printString("\n");					// Add '\n' (Python can know end of data)  
		_delay_ms(500);
    }
}

