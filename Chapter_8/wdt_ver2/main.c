/*
	watchdogTimer.c
	
	Demonstrate watchdog timer 
	NOTE: hfuse = 0xde -> Bit WTON = 1
 */

#define F_CPU 12000000UL				// Clock CPU: 12MHz
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <avr/wdt.h>					// Take wdt_reset()

/*	Define device pins	*/
#define LED_DDR			DDRB
#define LED_PORT		PORTB
#define LED0			PORTB0
#define LED1			PORTB1


void WDT_ON()
{
	cli();			// disable all interrupts
	wdt_reset();	// reset counting time of watchdog -> Timer = 0s
	
	// Start timed sequence
	// This lines indicates that there is changes in bit configuring
	WDTCSR |= (1 << WDCE) | (1 << WDE);
	
	// Set new prescaler (time-out) bits WDP3...0 = 0111 (timeout = 2s)
	// Bit WDTON = 1, bit WDE = 1, bit WDIE = 0 -> Reset mode
	// (Interrupt mode if Bit WDTON = 1, bit WDE = 0, bit WDIE = 1)
	WDTCSR = (1 << WDE) | (1 << WDP0) | (1 << WDP1) | (1 << WDP2);
	
	sei();			// enable global interrupts
}

void WDT_OFF()
{
	cli();			// disable all interrupts
	wdt_reset();	// reset counting time of watchdog -> Timer = 0s
	
	// Start timed sequence
	// This lines indicates that there is changes in bit configuring
	WDTCSR |= (1 << WDCE) | (1 << WDE);
	// Disable watchdog timer
	WDTCSR = 0x00;
	
	sei();			// enable global interrupts
}

int main(void)
{
   WDT_ON();						/* Enable the watchdog timer */
   LED_DDR |= (1 << LED0);
   LED_PORT |= (1 << LED0);			/* Set LED0 pin to logic high */
   _delay_ms(1000);					/* Wait for 1 second */
   LED_PORT &= ~(1 << LED0);		/* Clear LED0 pin */

   while(1);						// Run for 1s -> Watchdog timeout -> Reset mode
}

