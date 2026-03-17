/*
	footstepDetector.c
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
#define CYCLE_DELAY		10					
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

int main(void)
{
	/*------------Inits-----------*/
    uint16_t lightsOutTimer = 0;
    uint16_t adcValue;
	uint16_t middleValue = 511;
	uint16_t highValue = 520;
	uint16_t lowValue = 500;
	uint16_t noiseVolume = 0;
	uint8_t padding = INITIAL_PADDING;
	LED_DDR = ((1 << LED0) | (1 << LED1) | (1 << SWITCH));
	initADC();
	initUSART();
	
	/*------------Loop event-------*/
    while (1) 
    {
		adcValue = readADC(PIEZO);			// read value from sensor Piezo
		
		 /* moving average -- tracks sensor's bias voltage */
		 middleValue = adcValue + middleValue - ((middleValue - 8) >> 4);
		 
		 /* Moving Average:
			+) Above middleValue: highValue ? Updated when adcValue > middleValue >> 4
			+) Below middleValue: lowValue  ? Updated when adcValue < middleValue >> 4
			NOTE: middleValue >> 4 = middleValue / 16
			because we always use 16 * middleValue for calculations.			*/
		 if (adcValue > (middleValue >> 4))
		 {
			 highValue = adcValue + highValue - ((middleValue - 8) >> 4);
		 }
		 if (adcValue < (middleValue >> 4))
		 {
			 lowValue = adcValue + lowValue - ((middleValue - 8) >> 4);
		 }
		 
		 /*	highValue – lowValue: background noise 
			? padding: Add a small margin to the background noise for calibration 
			? Prevent the sensor from being overly sensitive to minor changes.		*/
		 noiseVolume = highValue - lowValue + padding;
		 
		 /* Now check to see if ADC value above or below thresholds */
		 if (adcValue < ((middleValue - noiseVolume) >> 4))
		 {
			 LED_PORT = (1 << LED0) | (1 << SWITCH);       // one LED, switch 
			 lightsOutTimer = ON_TIME / CYCLE_DELAY;       // reset timer 
		 }
		 else if (adcValue > ((middleValue + noiseVolume) >> 4))
		 {
			 LED_PORT = (1 << LED1) | (1 << SWITCH);		// other LED, switch
			 lightsOutTimer = ON_TIME / CYCLE_DELAY;        // reset timer 
		 }
		 else 
		 {                            /* Nothing seen, turn off lights */
			 LED_PORT &= ~(1 << LED0);
			 LED_PORT &= ~(1 << LED1);						// both off                
			 if (lightsOutTimer > 0)						// time left on timer 
			 {						
				 lightsOutTimer--;
			 }
			 else											 // time's up 
			 {                                          
				 LED_PORT &= ~(1 << SWITCH);                 // turn switch off 
			 }
		 }
		 
		 #if USE_POT                          // optional padding potentiometer 
		 padding = readADC(POT) >> 4;         // modify padding through PC5
		 #endif
		 /* Serial output and delay */
		 /* ADC is 10-bits, recenter around 127 for display purposes */
		printString("\r\nADC value: ");
		printWord(adcValue - 512 + 127);
		printString("\r\nlowVale: ");
		printWord((lowValue >> 4) - 512 + 127);
		printString("\r\nhighVale: ");
		printWord((highValue >> 4) - 512 + 127);

		 _delay_ms(CYCLE_DELAY);
    }
	
}

