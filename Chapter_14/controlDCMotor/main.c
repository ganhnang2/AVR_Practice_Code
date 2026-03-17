/*
	controlDCMotor.c
	
 */ 

#define F_CPU	12000000UL
#include <avr/io.h>
#include <util/delay.h>
#include "USART_RS232.h"

/*	Define device pins	*/
#define MOTOR_DDR		DDRD
#define MOTOR_PORT		PORTD
#define MOTOR			PORTD6

#define LED_DDR			DDRB
#define LED_PORT		PORTB
#define LED0			PORTB0
#define LED1			PORTB1

#define SPEED_STEP_DELAY 2

// Initialize Fast PWM mode in Timer0 to control motor
static inline void initTimer0()
{
	TCCR0A |= (1 << WGM01) | (1 << WGM00);		// Fast PWM mode 8-bit
	TCCR0A |= (1 << COM0A1);					// non-inverting mode on OC0A pin (PD6)
	TCCR0B |= (1 << CS02) | (1 << CS00);		// prescaler: 1024 -> fPWM = 12MHz / 1024 / 256 = 46Hz 
}

int main(void)
{
    /*---------------Init-----------*/
	initTimer0();
	initUSART();
	MOTOR_DDR |= (1 << MOTOR);					// set as output mode
	LED_DDR |= (1 << LED0) | (1 << LED1);	
	printString("DC Motor Workout\r\n");
	
	uint8_t motorSpeed;
	
    while (1) 
    {
		// Enter the motor speed (0 - 255)
		motorSpeed = getNumber();
		
		// The motor speed gradually increases
		if (OCR0A < motorSpeed)
		{
			LED_PORT |= (1 << LED0);
			while (OCR0A < motorSpeed)
			{
				OCR0A++;
				_delay_ms(SPEED_STEP_DELAY);
			}
				
		}
		// The motor speed gradually decreases
		else 
		{
			LED_PORT |= (1 << LED1);
			while (OCR0A > motorSpeed)
			{
				OCR0A--;
				_delay_ms(SPEED_STEP_DELAY);
			}
		}
		
		// Turn the LEDs off to indicate that reaching the threshold
		LED_PORT = 0x00;
    }
}

