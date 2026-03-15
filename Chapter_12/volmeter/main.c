/*
	volmeter.c
	Continuously outputs voltage over the serial line.
	
	By combining ADC Noise Reduction sleep mode and 16x oversampling, we can actually get just a little 
	more than two decimal places of accuracy in our measurements, but not quite enough to justify reporting 
	the third decimal place.
	
	In the sketch, i will measure voltage 9V by connecting the ADC5 pins to the midpoint node of the 
	voltage divider circuit.
	Voltage divide circuit includes: 9V battery and 2 resistor 10kOhm +- 1%
	
	Source: Nguyen Hai Nam
 */ 

#define F_CPU		12000000UL					// clock CPU: 12MHz
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>							// for ADC sleep mode
#include <util/delay.h>					
#include <math.h>								// for round() and floor()
#include "USART_RS232.h"

#define REF_VCC					5.053			// measure either AVCC of the voltage on AREF and enter it here.
#define VOLTAGE_DIV_FACTOR		1.98			// measured division by voltage divider
#define OFFSET_COEFFICIENT		0.04			// offset: 0.04 -> division factor is between 1.98 and 2.02

void initADC(void)
{
	ADMUX |= (0x0F & PORTC5);					// choose ADC5 as input
	ADMUX |= (1 << REFS0);						// AVCC with external capacitor at AREF pin
	ADCSRA |= (1 << ADPS2) | (1 << ADPS1);		// prescaler = 64 -> clock ADC = 12MHz/64 = 187.5kHz
	ADCSRA |= (1 << ADEN);						// enable ADC
}

void setupADCSleepMode(void)
{
	set_sleep_mode(SLEEP_MODE_ADC);				// enable sleep mode: ADC noise reduction
	ADCSRA |= (1 << ADIE);						// enable ADC interrupt
	sei();										// enable global interrupt
}

/* EMPTY_INTERRUPT(ADC_vect): creating a quick, fake ISR that we can trigger when we need to 
	wake the processor up from sleep modes
	-> This ISR returns to wherever it was in the code 
	(i.e: In this sketch, it is code line "oversampledValue += ADC")	*/
EMPTY_INTERRUPT(ADC_vect);		

uint16_t oversample16x(void)
{
	uint16_t oversampledValue = 0;
	for (uint8_t i = 0; i < 16; i++)
	{	
		sleep_mode();							// operate in sleep mode
		oversampledValue += ADC;				// after CPU wakes up from interrupt, add them up 
	}
	
	return (oversampledValue >> 2);				// divide / 4 to have resolution 12-bit
}		

void printFloat(float number) {
	number = round(number * 100) / 100;									// round off to 2 decimal places 
	transmitByte('0' + number / 10);									// tens place
	transmitByte('0' + number - 10 * floor(number / 10));				// ones place
	transmitByte('.');
	transmitByte('0' + (number * 10) - floor(number) * 10);				// first decimal digit
	transmitByte('0' + (number * 100) - floor(number * 10) * 10);		// second decimal digit
}		

int main(void)
{
	float voltage;
	
	/*-----------Inits----------*/
    initUSART();
	printString("\r\nDigital Voltmeter\r\n\r\n");
	initADC();
	setupADCSleepMode();
	
	/*----------Event loop-------*/
    while (1) 
    {
		/* Voltage range of battery	*/
		voltage = oversample16x() * VOLTAGE_DIV_FACTOR * REF_VCC / 4096;							
		printFloat(voltage);
		printString("-");
		_delay_ms(20);
		voltage = oversample16x() * (VOLTAGE_DIV_FACTOR + OFFSET_COEFFICIENT) * REF_VCC / 4096;	
		printFloat(voltage);
		printString("\r\n");
		
		_delay_ms(500);
    }
}

