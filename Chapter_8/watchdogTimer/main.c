/*
	watchdogTimer.c
	
	Demonstrate watchdog timer 
	NOTE: h fuse = 0xde -> Bit WTON = 1
 */

#define F_CPU 12000000UL				// Clock CPU: 12MHz
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <avr/wdt.h>				// Take wdt_reset()

/*	Define device pins	*/
#define LED_DDR			DDRB
#define LED_PORT		PORTB
#define LED0			PORTB0
#define LED1			PORTB1

#define BUTTON_DDR		DDRC
#define BUTTON_PORT		PORTC
#define BUTTON_PIN		PINC
#define BUTTON			PINC0

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
	
	// Clear WDRF in MCUSR 
	MCUSR &= ~(1 << WDRF);
	
	// Start timed sequence
	// This lines indicates that there is changes in bit configuring
	WDTCSR |= (1 << WDCE) | (1 << WDE);
	// Disable watchdog timer
	WDTCSR = 0x00;
	
	sei();			// enable global interrupts
}

int main(void)
{
	WDT_OFF();										// disable watchdog to prevent default timeout = 15ms
    LED_DDR |= (1 << LED0) | (1 << LED1);			// set output
	BUTTON_PORT |= (1 << BUTTON);					// enable resistor pull up
	LED_PORT &= ~(1 << LED0);
	
	/* wait until button is pressed */
	while (BUTTON_PIN & (1 << BUTTON));				
	
	/*	The moment button is pressed, enable watchdog timer	
		LED0 indicates that watchdog timer is active		*/
	LED_PORT |= (1 << LED0);									
	WDT_ON();							// from this moment, program will reset after 2 seconds		
	
	/*	Blinking LED when button is pressed */
	while(!(BUTTON_PIN & (1 << BUTTON)))
	{
		/*	NOTE: Placing wdt_reset() in the loop will cause undefined behavior 
			-> Can't run simulation on Proteus	*/
		LED_PORT ^= (1 << LED1);	
		_delay_ms(100);	
	}
	
			
	/*	hang program to allow WDT to time out */
    while (1);
}

