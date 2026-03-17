/*
	reactionTimer.c
	
	Press the button as quickly as you can after the LEDs light up.
	Your time is printed out over the serial port.
 */ 

#define F_CPU 12000000UL				// Clock CPU: 12MHz
#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>
#include "USART_RS232.h"
#include "support.h"

/*	Defines device pins	*/
#define BUTTON_PORT		PORTC
#define BUTTON_DDR		DDRC
#define BUTTON_PIN		PINC
#define BUTTON			PORTC0
#define LED_PORT		PORTB
#define LED_DDR			DDRB
#define LED0			PORTB0

static inline void initTimer1()
{
	TCCR1B |= (1 << CS12);					// normal mode, prescaler: 256
	
	/*	clock: 12MHz / 256 -> duty cycle: 21.33us -> 47 cycles per 1ms	*/
}

int main(void)
{
	uint16_t timerValue;						// reaction timer 
	
	/*---------------Inits-------------*/
	initUSART();
	initTimer1();
	srand(0);
	LED_DDR |= (1 << LED0);						// output mode
	BUTTON_PORT |= (1 << BUTTON);				// enable resistor pull up
	
	printString("Reaction Timer!\r\n");
	printString("----------------\r\n");
	printString("Press any key to start.\r\n");
	
    /*-----------Loop events------------*/
    while (1) 
    {
		receiveByte();							// press any key to start
		printString("\r\nGet Ready....");
		randomDelay();
	
		printString("\r\nGo!\r\n");			
		LED_PORT |= (1 << LED0);				// light up LED
		TCNT1 = 0;								// reset counter
		
		if (!(BUTTON_PIN & (1 << BUTTON)))
			/*	Check if you are already pressing the button and issues an warning*/
			printString("You're only cheating yourself.\r\n");
		else
		{
			while((BUTTON_PIN & (1 << BUTTON)));				// wait until the button is pressed
			
			/*	TCNT1: the amounts of clock cycles,	1 ms has 47 cycles
				-> timerValue (ms) = total clock cycles / (47 cycles per 1 ms)	*/
			timerValue = (uint16_t) (TCNT1 / 47);							// timerValue (ms)
			
			printMilliseconds(timerValue);									// print your reaction by seconds
			printComments(timerValue);										// message to you reaction
		}
		
		// Clear LED and start again
		LED_PORT = 0x00;
		printString("Press any key to try again.\r\n");
    }
}

