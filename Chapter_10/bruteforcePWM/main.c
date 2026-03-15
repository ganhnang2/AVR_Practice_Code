/*
	bruteforcePWM.c
	Create PWM waveform manually with function delay
 */ 

#define F_CPU	12000000UL				// Clock CPU: 12MHz
#include <avr/io.h>
#include <util/delay.h>

/*	Define device pins	*/
#define LED_DDR			DDRB
#define LED_PORT		PORTB
#define LED0			PORTB0

// The PWM frequency:
// f = 1s / (256 * DELAY_US) ~= 195Hz
#define DELAY_US		20				

void pwmAllPins(uint8_t brightness)
{
	// NOTE: i < 255 not 256
	// If i < 256, this will cause output LOW continously
	// Explain for case that i < 256: 
	// brightness = 1 (from main)
	// At the 1th iteration, i = 0 -> OUTPUT = HIGH
	// At the 2th iteration, i = 1 -> OUTPUT = LOW (continously)
	// .......
	// When the 255th iteration occurs, i = 255 and brightness = 1 -> OUTPUT = LOW
	// i++ -> i = 255 + 1 -> Overflow -> i = 0 -> Run for() loop repeatly from the scratch -> Infinitive loop
	uint8_t i;
	LED_PORT = 0xFF;						// Time for HIGH pulse
	for (i = 0; i < 255; i++)
	{
		if (i >= brightness)				// Time for LOW pulse
			LED_PORT = 0x00;
		_delay_us(DELAY_US);
	}
}

int main(void)
{
    /*-------------Inits----------*/
	LED_DDR = 0xFF;			
	
	uint8_t brightness = 0;
	int8_t direction = 1;
	
	/*---------Loop events--------*/
    while (1) 
    {
		if (brightness == 0)			// increase the LED light intensity
			direction = 1;
		else if (brightness == 255)		// decrease the LED light intensity
			direction = -1;
			
		brightness += direction;
		pwmAllPins(brightness);
    }
	
	return 0;
}

