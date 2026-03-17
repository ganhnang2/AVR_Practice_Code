/*
	spi_slave.c
	
	SS pin is wired up to PORTB2
 */ 

#define F_CPU		12000000UL		// Clock CPU: 12MHz
#include <avr/io.h>
#include <stdio.h>
#include "lcd16x02.h"
#include "SPI_Slave.h"
#include <util/delay.h>

int main(void)
{
    /*---------Inits----------*/
    uint8_t count;
    char buffer[5];
    
    lcdBegin();
    SPI_Init();
    
    printStringAtAnyPosition(0, 0, "Slave Device");
    printStringAtAnyPosition(1, 0, "Receive Data: ");
 
    /*--------Loop event------------*/
    while (1)
    { 
		count = SPI_Receive();
		sprintf(buffer, "%d", count);
		printStringAtAnyPosition(1, 13, buffer);	
    }
}

