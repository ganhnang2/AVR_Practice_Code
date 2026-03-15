/*
	progremdemo1.c
 */ 

#define F_CPU		12000000UL			// Clock CPU: 12MHz
#include <avr/io.h>
#include <avr/pgmspace.h>
#include <util/delay.h>
#include "USART_RS232.h"

/*	NOTE: When declaring program-space “variables” in C, we must obey two rules:
	1. Always preface keyword const
	2. Variables in program-space must be global variable
*/
const char myString[] PROGMEM = "\r\n Hi there, this is an example for using program-space flash memory!\r\n";
const uint16_t value PROGMEM = 12345;

int main(void)
{
    initUSART();
	char oneLetter;
	uint16_t i;
	
    while (1) 
    {
		for (i = 0; i < sizeof(myString); i++)
		{
			oneLetter = pgm_read_byte(&(myString[i]));
			transmitByte(oneLetter);
			_delay_ms(100);
		}
		_delay_ms(1000);
		
		printWord(&value);
		printString("\r\n");
		printWord(pgm_read_word(&value));
    }
}

