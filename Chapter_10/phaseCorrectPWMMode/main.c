/*
	phaseCorrectPWMMode.c
	Type a random number to control the LED brightness with phase correct PWM mode
	The phase correct PWM mode is, like the phase and frequency correct PWM mode, based on a dual-slope operation.
	Application: Due to symmetric feature of the dual-slope PWM modes, these modes are preferred for motor
	control applications.
	
	1. The PWM resolution for fast PWM can be fixed to 8-, 9-, or 10-bit, or defined by either ICR1 or OCR1A
					log(TOP + 1)
	Resolution = ------------------------
						log(2)
	-> TOP value = pow(2, resolution) - 1
	
	2. The PWM frequency for the output can be calculated by the following equation:
					fclk_I/O
	fOCnxPWM = ----------------------
					2 * N * TOP
	The N variable represents the prescaler divider (1, 8, 64, 256, or 1024).
	
	Special cases for non-inverted PWM mode:
	1. If OCR1x = 0x0000 (BOTTOM), the output will be continuously LOW (duty cycle = 0%)
	2. If OCR1x = TOP, the output will be continuously HIGH (duty cycle = 100%)
 */ 

#define F_CPU	12000000UL				// Clock CPU: 12MHz
#include <avr/io.h>
#include <util/delay.h>
#include "USART_RS232.h"

#define LED_DDR			DDRB
#define LED_PORT		PORTB
#define LED1			PORTB1			// pin OC1A of Timer1
#define LED2			PORTB2			// pin OC1B of Timer1
#define LED3			PORTB2			// pin OC2A of Timer2

static inline void pwmInit()
{
	/*	Timer1	*/
	// WGM13:0 =  1 -> phase correct PWM mode 8-bit
	// COM1A1:0 = 2, COM1B1:0 = 2 -> enable OC1A, OC1B pin
	TCCR1A |= (1 << COM1A1) | (1 << COM1B1) | (1 << WGM10);			
	
	// CS12:0 = 2 -> prescaler: 8		
	// The PWM frequency: f = 12MHz / (2 * 8 * 255) = 2941Hz					
	TCCR1B |= (1 << CS11);
	
	/*	Timer2	*/
	// WGM23:0 =  1 -> phase correct PWM mode 8-bit
	// COM2A1:0 = 2 -> enable OC2A
	TCCR2A |= (1 << COM2A1) | (1 << WGM20);
	
	// CS22:0 = 2 -> prescaler: 8
	// The PWM frequency: f = 12MHz / (2 * 8 * 255) = 2941Hz
	TCCR2B |= (1 << CS21);
}

int main(void)
{
	/*--------------Inits-------------*/
	pwmInit();
	initUSART();
	LED_DDR |= (1 << LED1) | (1 << LED2) | (1 << LED3);		// set OC1A/B, OC2A as output mode
	printString("----LED PWM Demo----\r\n");
	
	uint8_t brightness;				// the LED light intensity (0 - 255)
	
    /*---------Loop event---------*/
    while (1) 
    {
		 printString("\r\nEnter (0-255) for PWM duty cycle: ");
		 brightness = getNumber();				// receive a number through serial port
		 
		 // use these register OCRxn to store previous variable
		 OCR1A = OCR1B;
		 OCR1B = OCR2A;
		 OCR2A = brightness;
    }
}

