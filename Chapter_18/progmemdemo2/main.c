/*
	progmemdemo2.c
 */ 

#define F_CPU		12000000UL
#include <avr/io.h>
#include <avr/pgmspace.h>
#include <util/delay.h>
#include "USART_RS232.h"

const char myString[] PROGMEM = "\r\n Hi there, this is an example for using program-space flash memory!\r\n";
const char myString2[] PROGMEM = "Demonstrate passing pointer to functions!\r\n";

void printString_PROGMEM(const char* strP)
{
	char oneLetter;
	while ((oneLetter = pgm_read_byte(strP)))
	{
		transmitByte(oneLetter);
		strP++;
		_delay_ms(100);
	}
}

int main(void)
{
	initUSART();
	
	while (1)
	{
		printString_PROGMEM(&myString[0]);
		printString_PROGMEM(&myString2[0]);
		_delay_ms(1000);
	}
}

