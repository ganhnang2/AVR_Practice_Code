/*
	helloInterrupt.c
	
	Demo of using interrupts for doing what they do best two things at once
	Flashes LED0 at a fixed rate, interrupting whenever button is pressed.
*/ 

#define F_CPU 8000000UL					// Clock CPU: 8MHz
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

/*	Defines device pins	*/
#define BUTTON_PORT			PORTC
#define BUTTON_PIN			PINC
#define BUTTON				PINC0
#define LED_DDR				DDRB
#define LED_PORT			PORTB
#define LED0				PORTB0
#define LED1				PORTB1

/*	Trigger INT0 interrupts	
	Run every times there is a change on button	*/
ISR(INT0_vect)
{
	/*	Turn on/off LED */
	if (BUTTON_PIN & (1 << BUTTON))
		LED_PORT |= (1 << LED1);
	else
		LED_PORT &= ~(1 << LED1);
}

/*	Initialize interrupt	*/
void initInterrupt0(void)
{
	EIMSK |= (1 << INT0);					// enable INT0 interrupt
	EICRA |= (1 << ISC00);					// low-level signal to active interrupt
	sei();									// NOTE: Always set global interrupt enable bit
}

int main(void)
{
	/*-------------Inits------------*/	
	LED_DDR |= ((1 << LED0) | (1 << LED1));	// set PB0, PB1 output
	BUTTON_PORT |= (1 << BUTTON);			// enable resistor pull-up on PC0
	
	initInterrupt0();
	
	/*-------------Event loop------------*/	
    while (1) 
    {
		/*	Blink LED on LED0 */
		LED_PORT ^= (1 << LED0);
		_delay_ms(200);
    }
}

