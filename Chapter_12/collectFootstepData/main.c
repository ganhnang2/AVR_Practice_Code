/*
	footstepDetector_fix.c
	Sensitive footstep-detector and EWMA demo
	
	We will be using the ADC and a sensor to make essentially a DIY accelerometer, but one that is specifically
	tuned to detect very small vibrations like taps on a tabletop, tiny earthquakes, or in my case, a person 
	walking up the stairs.
	
	+) The circuit:
										VCC
										|
										|
										R1 (1Mohm)
	AVR PC2								|
	----------R3(10 kOhm)---------------|----------------
										|				|
										R2 (1Mohm)		Piezo Element
										|				|
										|				|
										GND				GND
	
	
 */ 

#define F_CPU		12000000UL				// Clock CPU: 12MHz
#include <avr/io.h>
#include <util/delay.h>
#include <avr/sleep.h>
#include "USART_RS232.h"

/*	Define device pins	*/
#define	LED_DDR		DDRB
#define LED_PORT	PORTB
#define LED0		PORTB0
#define LED1		PORTB1

#define PIEZO		PORTC2
#define SWITCH		PORTB2					// turn on when detecting footstep with LED0/1
#define	USE_POT		0						// if using pot, define USE_POT to 1
#if USE_POT
#define	POT			PORTC5
#endif

/*	Define constant symbolic	*/
#define ON_TIME			2000				// time (ms) for turning on LED
#define CYCLE_DELAY		500					
#define INITIAL_PADDING	16					// higher is less sensitive


void initADC(void)
{
	ADMUX |= (1 << REFS0);						// AVCC with external capacitor at AREF pin
	ADCSRA |= (1 << ADPS2) | (1 << ADPS1);		// prescaler = 64 -> clock ADC = 12MHz/64 = 187.5kHz
	ADCSRA |= (1 << ADEN);						// enable ADC
}

uint16_t readADC(uint8_t channel)
{
	ADMUX = (0xF0 & ADMUX) | channel;			// choose ADC5 as input
	ADCSRA |= (1 << ADSC);						// start ADC conversion
	while (ADCSRA & (1 << ADSC));				// loop until complete conversion
	return (ADC);
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
	/*------------Inits-----------*/
    // uint16_t lightsOutTimer = 0;
    uint16_t adcValue;
	float middleValue = 511;
	float highValue = 520;
	float lowValue = 500;
	// float noiseVolume = 0;
	// uint8_t padding = INITIAL_PADDING;
	LED_DDR = ((1 << LED0) | (1 << LED1) | (1 << SWITCH));
	initADC();
	initUSART();
	
	/*------------Loop event-------*/
    while (1) 
    {
		adcValue = readADC(PIEZO);			// read value from sensor Piezo
		
		 /* moving average -- tracks sensor's bias voltage */
		 middleValue = (float)(adcValue / 16) + (15 * middleValue) / 16;
		 
		 /* Moving Average:
			+) Above middleValue: highValue ? Updated when adcValue > middleValue
			+) Below middleValue: lowValue  ? Updated when adcValue < middleValue		*/
		 if (adcValue > middleValue)
		 {
			 highValue = (float)(adcValue / 16) + (15 * highValue) / 16;
		 }
		 if (adcValue < middleValue)
		 {
			 lowValue = (adcValue / 16) + (15 * lowValue) / 16;;
		 }
		 
		 /*	highValue – lowValue: background noise 
			? padding: Add a small margin to the background noise for calibration 
			? Prevent the sensor from being overly sensitive to minor changes.		*/
		 // noiseVolume = highValue - lowValue + padding;
		
		 /* Serial output and delay */
		 printString("\r\nADC value: ");
		 printWord(adcValue);
		 printString("\r\nlowVale: ");
		 printFloat(lowValue);
		 printString("\r\nhighVale: ");
		 printFloat(highValue);
		 _delay_ms(CYCLE_DELAY);
	}
	
}

