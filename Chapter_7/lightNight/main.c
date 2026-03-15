/*
	nightLight.c
	
	Using POT as threshold for LDR value and then, turning LED ON/OFF.
 */ 

#define F_CPU 12000000UL				// Clock CPU: 12MHz
#include <avr/io.h>
#include <util/delay.h>
#include "USART_RS232.h"

/*	Define pins	*/
#define LED_DDR			DDRB
#define LED_PORT		PORTB
#define LED0			PORTB0
#define POT				3			// ADC3
#define LIGHT_SENSOR	0			// ADC0

// ADC value for a specific channel
uint16_t readADC(uint8_t channel)
{
	ADMUX = (0xF0 & ADMUX) | channel;	// channel is read
	ADCSRA |= (1 << ADSC);				// Start ADC conversion
	while (ADCSRA & (1 << ADSC));		// Loop until ADC conversion is complete!
	return (ADC);
}

int main(void)
{
	/*---------------Inits-----------------*/
	uint16_t lightThreshold;
	uint16_t sensorValue;
	
	initUSART();
	
	// Set up LED
	LED_DDR = (1 << LED0);
	
	// Set up ADC
	ADMUX |= (1 << REFS0);			// AVCC with external capacitor at AREF pin
	ADMUX &= ~(1 << ADLAR);			// Clear bit ADLAR -> Right Adjust
	ADCSRA |= (1 << ADEN) | (1 << ADPS2) | (1 << ADPS1);		// ADC prescale = 64 -> ADC clock = 12MHz / 64 = 187.5kHz
																// ADC enable
    /*---------------Event loop-----------------*/
    while (1) 
    {
		// ADC values 
		lightThreshold = readADC(POT);
		sensorValue = readADC(LIGHT_SENSOR);
		
		// Display on RealTerm to check
		printWord(lightThreshold);
		printString("\r\n");
		printWord(sensorValue);
		printString("\r\n");
		
		// Control LED
		if (sensorValue < lightThreshold)
			PORTB |= (1 << LED0);
		else
			PORTB &= ~(1 << LED0);
			
		_delay_ms(100);
    }
	
	return 0;
}

