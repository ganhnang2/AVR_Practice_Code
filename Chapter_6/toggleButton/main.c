/*
	Demonstrates using state to detect button presses
 */ 
#define F_CPU 8000000UL			// Clock CPU: 8MHz
#include <avr/io.h>
#include <util/delay.h>

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
	uint8_t wasButtonPressed = 0;
	
	//------------------Loop event-------------------//
    while (1) 
	{
		/*	If button is pressed now, ...	*/
		if (!(BUTTON_PIN & (1 << BUTTON)))			
		{
			if (!wasButtonPressed)			// Previous button state isn't pressed
			{
				LED_PORT ^= (1 << LED);		// Toggle LED state
				wasButtonPressed = 1;		// Update button state
			}
		}
		/*	Button isn't pressed now		*/
		else
			wasButtonPressed = 0;			// Update the state
    }
	
	return 0;
}

