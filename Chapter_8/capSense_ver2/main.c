/*
	capSense.c
	
	Capacitive touch sensor demo
	Using the direct method, to time directly the capacitor discharging with nobody touch it, then with
	someone touching it. Finally, compute the difference in time.
	NOTE: Version don't use interrupt, implement sketch by Polling
	
	Circuit:						Capacitive Sensor (Cooper)
	|-------------------|				|
	|					|	PC0 (I/O)	|
	|		MCU			|----------------	Resistor (500kohm)------------- GND
	|					|
	|-------------------|
	
	
	chargeCycleCount (us)
	+) Not touch: 5
	+) Touch: 2000 - 5000
*/ 

#define F_CPU 12000000UL			// Clock CPU: 12MHz
#include <avr/io.h>
#include <util/delay.h>
#include <avr/power.h>
#include "USART_RS232.h"

/*	Defines device pins	*/
#define LED_DDR				DDRB
#define LED_PORT			PORTB
#define LED0				PORTB0
#define CAP_SENSOR_DDR		DDRC
#define CAP_SENSOR_PORT		PORTC
#define CAP_SENSOR			PORTC0
#define CAP_SENSOR_PIN		PINC

/*	Define constant	*/
#define THRESHOLD			2000					// 2000uS

int main(void)
{
    /*------------Inits-------------*/
	uint16_t chargeCycleCount = 0;
	clock_prescale_set(clock_div_1);
	initUSART();
	LED_DDR |= (1 << LED0);							// PORTB: output
	MCUCR |= (1 << PUD);							// disable all resistors pullup

	CAP_SENSOR_DDR |= (1 << CAP_SENSOR);			// start with cap charged
	CAP_SENSOR_PORT |= (1 << CAP_SENSOR);			// output HIGH
	
    while (1) 
    {	
		CAP_SENSOR_DDR &= ~(1 << CAP_SENSOR);		// discharged
		while (CAP_SENSOR_PIN & (1 << CAP_SENSOR))	// loop until capacitor is fully discharged
		{
			chargeCycleCount++;						// count this change
			_delay_us(1);
		}
			
		// compare
		if (chargeCycleCount > THRESHOLD)
			LED_PORT |= (1 << LED0);
		else
			LED_PORT &= ~(1 << LED0);
		
		// display data to check
		printWord(chargeCycleCount);
		printString("\r\n");
		
		chargeCycleCount = 0;						// reset counter
		CAP_SENSOR_DDR |= (1 << CAP_SENSOR);		// start with cap charged
		_delay_us(100);								// time for charging
    }
}

