/*
	pwmOnAnyPin.c
	We will use the interrupts to trigger our own code, and turn on and off PWM pins
	from within ISRs.
	We can use a timer/counter in Normal mode to do the counting for us
	+) At the beginning of the cycle, turn on PWM pins in ISR
	+) When reaching the output-compare values, triggering another interrupt to turn the pins back off
	
	NOTE: PWM values are long enough that the ISRs have time to execute
	-> Recommend: Clock prescaler >= 64
 */ 

#define F_CPU		12000000UL			// Clock CPU: 12MHz
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#define LED_DDR			DDRB
#define LED_PORT		PORTB
#define LED0			PORTB0		
#define LED1			PORTB1	

#define DELAY	3

volatile uint8_t brightnessA;
volatile uint8_t brightnessB;

static inline void initTimer0()
{
	// normal mode
	TCCR0A |= 0x00;
	
	// CS02:0 = 4 -> prescaler: 64
	// The PWM frequency: f = 12MHz / (2 * 64 * 255) = 368Hz
	TCCR0B |= (1 << CS12);
	
	// enable timer overflow and output compare OCR0A/OCR0B interrupt
	TIMSK0 |= (1 << OCIE0A) | (1 << OCIE0B) | (1 << TOIE0);
	sei();			// enable global interrupt
}

ISR(TIMER0_OVF_vect)
{
	LED_PORT |= (1 << LED0) | (1 << LED1);
	OCR0A = brightnessA;
	OCR0B = brightnessB;
}

ISR(TIMER0_COMPA_vect)
{
	LED_PORT &= ~(1 << LED0);					// turn off LED0
}

ISR(TIMER0_COMPB_vect)
{
	LED_PORT &= ~(1 << LED1);					// turn off LED1
}


int main(void)
{
	/*----------------Inits--------------*/
   
	uint8_t i;
	LED_DDR |= (1 << LED0) | (1 << LED1);
	initTimer0();
   
	/*----------------Event loop--------------*/
    while (1) 
    {
		for (i = 0; i < 255; i++)		
		{
			_delay_ms(DELAY);
			brightnessA = i;			// increase the LED0 light intensity
			brightnessB = 255 - i;		// decrease the LED1 light intensity
		}
		
		for (i = 254; i > 0; i--)
		{
			_delay_ms(DELAY);
			brightnessA = 255 - i;		// decrease the LED0 light intensity
			brightnessB = i;			// increase the LED1 light intensity
		}
		
    }
}

