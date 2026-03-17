/*
 * Blink_LED.c
 *
 * Created: 12/15/2025 12:04:58 AM
 * Author : ASUS
 */ 
#define F_CPU 8000000UL
#include <avr/io.h>
#include <util/delay.h>


int main(void)
{
      // -------- Inits --------- //
  DDRD |= 0xFF;				/* Data Direction Register B:
                                   writing a one to the bit
                                   enables output. */
  PORTD = 0x00;
  // ------ Event loop ------ //
  while (1) {
    PORTD = 0xFF;          /* Turn on first LED bit/pin in PORTB */
	_delay_ms(500);
	
	PORTD = 0x00;
	_delay_ms(500);
 
  }                                                  /* End event loop */
  return 0;                            /* This line is never reached */
}

