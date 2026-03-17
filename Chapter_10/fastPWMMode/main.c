/*
	fastPWMMode.c
	In the sketch, i will use fast PWM mode to create PWM on OC1x pin.
	Source: Nguyen Hai Nam
	
	Pulse Width Modulation (PWM) is a technique by which the width of a pulse is varied while keeping the frequency constant.
	During PWM operations, Timer 1 counts from zero up to a top value and back down again to zero.
	The top value is determined by the resolution desired.
	
	1. The PWM resolution for fast PWM can be fixed to 8-, 9-, or 10-bit, or defined by either ICR1 or OCR1A
						log(TOP + 1)
	Resolution = ------------------------
						log(2)
	-> TOP value = pow(2, resolution) - 1
	
	2. The PWM frequency for the output can be calculated by the following equation:
					fclk_I/O
	fOCnxPWM = ----------------------
					N * (1 + TOP)
	The N variable represents the prescaler divider (1, 8, 64, 256, or 1024).
	
	+) With TOP value is determined by WGM13:0 bits., TOP = 
	- 0x0FF, 0x1FF, 0x3FF if WGM13:0 = 5, 6, or 7
	- ICR1 if WGM13:0 = 14
	- OCR1 if WGM13:0 = 15
	+) There are two choices for fast PWM mode (CS11:0 bit): non-inverting and inverting
	- non-inverting: clear OC1x on compare match, set OC1x at 0x0000
	- inverting: set OC1x on compare match, clear OC1x at 0x0000
	
	Example: Non-inverting mode
	Setting Duty cycle: we have to load value in the OCR0 register to set the duty cycle.
	255 value for 100% duty cycle and 0 for 0% duty cycle. Accordingly, if we load value 127 in OCR0, the Duty cycle will be 50%.
 */				

/*
	In short, steps to produce a custom waveform on an output compare pin OC1x are as follows:
	1. Select the operation mode of Timer 1: CTC, Fast PWM, or Phase Correct PWM, …
	2. Select how output compare pin OC1x will be updated on compare match event (COM1A1:0, COM1B1:0)
	3. Configure timer1: clock source, prescaler, ...
	4. Put correct values in the output compare registers (OCR1x).
*/

#define F_CPU	12000000UL				// Clock CPU: 12MHz
#include <avr/io.h>
#include <util/delay.h>

/*	Control LED on OC1A pin	*/
#define LED_DDR			DDRB
#define LED_PORT		PORTB			
#define LED0			PORTB1

static inline void PWM_init()
{
	// WGM13:0 = 5 -> Fast PWM mode with 8-bit resolution
	// COM1A1:0 = 2 -> Non-inverting mode in OC1A pin
	// COM1B1:0 = 0 -> Disconnect OC1B pin
	TCCR1A |= (1 << COM1A1) | (1 << WGM10);
	
	// prescaler = 64 with external clock 12MHz
	// The PWM frequency:
	//			12MHz
	// f = ------------------- = 732Hz
	//		 64 * (1 + 255)
	TCCR1B |= (1 << WGM12) | (1 << CS11) | (1 << CS10);
}


int main(void)
{
    /*--------Inits---------*/
	uint8_t duty;
	PWM_init();
	LED_DDR |= (1 << LED0);		// set OC1A pin as output
	
    while (1) 
    {
		
		for (duty = 0; duty < 255; duty++)
		{
			OCR1A = duty;			// increase the LED light intensity
			_delay_ms(10);
		}
		
		for (duty = 255; duty > 0; duty--)
		{
			OCR1A = duty;			// decrease the LED light intensity
			_delay_ms(10);
		}

    }
}

