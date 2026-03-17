/*
	bossButton.c
	
	Doan code nay se doc trang thai cua button va gui ky tu tuong ung
	qua cong Serial cho Script Python de xu ly
 */ 
#define F_CPU 12000000UL			// Clock CPU: 12MHz
#include <avr/io.h>
#include <util/delay.h>
#include "USART_RS232.h"

/*	Define LED pin	*/
#define LED_DDR			DDRD
#define LED_PORT		PORTD
#define LED				PORTD7

/*	Define Button pin	*/
#define BUTTON_DDR		DDRB
#define BUTTON_PORT		PORTB
#define BUTTON_PIN		PINB
#define BUTTON			PORTB0

int main(void)
{
	//------------------Inits-------------------//
	BUTTON_PORT |= (1 << BUTTON);			// Enable Rpullup in BUTTON
	LED_DDR = (1 << LED);					// Configure LED output
	
	initUSART();
	transmitByte('O');						// Message: AVR is ready to transmit
	
	//------------------Loop event-------------------//
    while (1) 
	{
		/*	If button is pressed now, ...	*/
		if (!(BUTTON_PIN & (1 << BUTTON)))			
		{
			while(!(BUTTON_PIN & (1 << BUTTON)));
			transmitByte('X');
			LED_PORT ^= (1 << LED);		// Toggle LED state
		}
    }
	
	return 0;
}

