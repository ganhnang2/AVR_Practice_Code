/*
	lightSensor.c
	
	Using ADC to measure analog signal from LDR
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
#define LED3		PORTB3

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
	uint8_t ledValue;
	initADC0();
	initUSART();
	
	LED_DDR |= ((1 << LED0) | (1 << LED1) | (1 << LED2) | (1 << LED3));
	
    /* Replace with your application code */
    while (1) 
    {
		ADCSRA |= (1 << ADSC);				// start ADC conversion
		while (ADCSRA & (1 << ADSC));		// Loop until ADC conversion completes
		adcValue = ADC;						// only read ADCH when ADC is right-adjusted
		
		/*	Have 10 bits but only 2 bit to control 4 LED	*/
		ledValue = (adcValue >> 8);
		
		// Reset pin
		LED_PORT = 0x00;	
		
		// Display LED
		for (uint8_t i = 0; i <= ledValue; i++)
		{
			LED_PORT |= (1 << i);
			_delay_ms(5);
		}
		
		/*	Can use to display data on RealTerm	*/
		printWord(adcValue);					// Transmit ADC Value
		printString("\r\n");					
		printByte(ledValue);					// Transmit ledValue
		printString("\r\n");
		_delay_ms(500);
    }
}

