/*
 * Simualtion_test.c
 *
 * Created: 1/30/2026 12:05:27 AM
 * Author : ASUS
 */ 

#define F_CPU	12000000UL
#include <avr/io.h>
#include <util/delay.h>
#include "USART_RS232.h"

int main(void)
{
	uint8_t c;
	initUSART();
	
    while (1) 
    {
		printString("Enter number: ");
		c = receiveByte();
		printString("\r\n");
		printString("Received: ");
		transmitByte(c);
		printString("\r\nDONE!\r\n");
		_delay_ms(1000);
    }
}

