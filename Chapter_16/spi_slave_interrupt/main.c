/*
	spi_slave_interrupt.c
 */ 
#define F_CPU		12000000UL		// Clock CPU: 12MHz
#include <avr/io.h>
#include <stdio.h>
#include <util/delay.h>
#include "lcd16x02.h"
#include "SPI_Slave_Interrupt.h"

#define SLAVE_ID			0x00

int main(void)
{
   /*---------Inits----------*/
   char data = 0;
   char buffer[10];
   
   
   SPDR = data;		// load first byte b/c interrupt can only be run after first byte
   
   lcdBegin();
   SPI_Init();
   sei();			// enable global interrupt

   /*--------Loop event------------*/
   while (1)		/*	Sending data to master	*/
   {
	   data = SPI_Receive();
	   sprintf(buffer, "%d", data);
	   printStringAtAnyPosition(0, 0, buffer);
   }
}

