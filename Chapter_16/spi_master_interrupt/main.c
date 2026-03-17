/*
 * spi_master_interrupt.c
 *
 * Created: 1/28/2026 12:33:48 PM
 * Author : ASUS
 */ 

#define F_CPU	12000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include "SPI_Master.h"
#include "lcd16x02.h"


int main(void)
{
	/*---------Inits----------*/
	uint8_t slaveID = 0;
	uint8_t data = 0;
	char buffer[5];
	
	lcdBegin();
	SPI_Init();
	sei();									// enable global interrupt
	// SPDR = 0xFF;							// write first data to slave for active slave
	SS_PORT &= ~(1 << slaveID);				// SS = LOW -> SPI is active -> Transfer data with salveID
	/*--------Loop event------------*/
	while (1)			/* Receive Continuous count */
	{
		SPI_Transmit(slaveID, data);
		sprintf(buffer, "%d", data);
		printStringAtAnyPosition(0, 0, buffer);
		data++;
		_delay_ms(500);
	}
}
