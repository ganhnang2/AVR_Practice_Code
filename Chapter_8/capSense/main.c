/*
	capSense.c
	
	Capacitive touch sensor demo
	Using the indirect method
*/ 

#define F_CPU 12000000UL			// Clock CPU: 12MHz
#include <avr/io.h>
#include <util/delay.h>
#include <avr/power.h>
#include <avr/interrupt.h>
#include "USART_RS232.h"

/*	Defines device pins	*/
#define LED_DDR				DDRB
#define LED_PORT			PORTB
#define LED0				PORTB0
#define CAP_SENSOR_DDR		DDRC
#define CAP_SENSOR_PORT		PORTC
#define CAP_SENSOR			PORTC0

/*	Define constant	*/
#define SENSE_TIME			200				// 200ms
#define THRESHOLD			20000			// 20000uS

//-----------Global variable-------------//
volatile uint16_t chargeCycleCount;

//----------Function-----------//
void initPCINT(void)
{
	PCICR |= (1 << PCIE1);					// enable PCINT1 (bank C)
	PCMSK1 |= (1 << PCINT8);				// use PC0 to request interrupt
}

ISR(PCINT1_vect)
{
	 chargeCycleCount++;                    // count this change
	 CAP_SENSOR_DDR |= (1 << CAP_SENSOR);	// charging delay
	 _delay_us(1);
	 
	 CAP_SENSOR_DDR &= ~(1 << CAP_SENSOR);	// set as input
	 PCIFR |= (1 << PCIF1);					// clear PCINT1
}

int main(void)
{
    /*------------Inits-------------*/
	clock_prescale_set(clock_div_1);
	initUSART();
	LED_DDR |= (1 << LED0);						// PORTB: output
	MCUCR |= (1 << PUD);						// disable all resistors pullup
	CAP_SENSOR_PORT |= (1 << CAP_SENSOR);		// output HIGH
	initPCINT();
	
    while (1) 
    {
		chargeCycleCount = 0;					// reset counter
		CAP_SENSOR_DDR |= (1 << CAP_SENSOR);	// start with cap charged
		sei();									// enable global interrupt -> counting chargeCycleCount
		_delay_ms(SENSE_TIME);
		cli();									// finish
		
		if (chargeCycleCount < THRESHOLD)
			LED_PORT |= (1 << LED0);
		else
			LED_PORT &= ~(1 << LED0);
			
		// compare to control LED
		printWord(chargeCycleCount);
		printString("\n");
    }
}

