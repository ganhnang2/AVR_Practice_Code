/*
	spi_master.c
	Let us first program the Master device (i.e: ATmega328P)
	
	+) The SS pins are respectively connected to the PORTDi (i = 0 ... 7) pins
	-> In the skecth, i will implement 3 slaves(PORTD0:2)
	+) All MOSI, MISO and SCK pins in a master and 3 slaves is connected to each other with a single wire
 */ 

#define F_CPU		12000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include "SPI_Master.h"
#include "lcd16x02.h"

int main(void)
{
	/*---------Inits----------*/
	uint8_t slaveID = 0;
	uint8_t slaveData[] = {0, 10, 20};
	char buffer[5];
	
	lcdBegin();
	SPI_Init();
	
	printStringAtAnyPosition(0, 0, "Master Device");
	printStringAtAnyPosition(1, 0, "Data to ");
	
    /*--------Loop event------------*/
    while (1)			/* Send Continuous count */
    {
		SPI_Transmit(slaveID, slaveData[slaveID]++);		// transmit data to slaveID
		sprintf(buffer, "slave: %d", slaveID);
		printStringAtAnyPosition(1, 8, buffer);
		
		slaveID++;		
		if (slaveID == 3)		// reset Slave ID
			slaveID = 0;
		
		_delay_ms(500);
    }
}

