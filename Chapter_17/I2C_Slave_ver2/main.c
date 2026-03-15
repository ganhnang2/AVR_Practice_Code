/*
	I2C_Slave_ver2.c

 */ 

#define F_CPU		12000000UL		// Clock CPU: 12MHz
#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include "TWI_Slave_ver2.h"
#include "lcd16x02.h"

#define SLAVE_ADDRESS		0x02	// Own slave address: 0x02

int main(void)
{
    /*---------------Inits--------------*/			
    char buffer[10];
	uint8_t dataTransmit[10];				// transmitted data to master in slave transmitter mode
	int8_t data = 0;
	uint8_t mode;							// 0: Transmitter, 1: Receiver
    I2C_Slave_Init(SLAVE_ADDRESS);
	lcdBegin();
	sei();									// enable global interrupt
	for (uint8_t i = 0; i < 10; i++)
		dataTransmit[i] = i;
	
	I2C_Slave_Transmit(dataTransmit, 10);
	printStringAtAnyPosition(0, 0, "Slave Device");
    while (1) 
    {
		mode = I2C_Slave_Mode();
		printStringAtAnyPosition(1, 0, mode ? "Receiving: " : "Sending: ");
		setCursor(1, 13);
		switch(mode)
		{
			/*----------Slave Transmitter Mode-----------*/
			case 0:
				
				data = I2C_Transmit_Status();
				if (data != -1)
				{
					sprintf(buffer, "%d", data);
					printString(buffer);
				}
				else				// check whether STOP/REAPEATED START condition is received
				{
					data = 0;
				} 
						
					
				break;
			/*----------End for Slave Transmitter Mode-----------*/
			
			/*----------Slave Receiver Mode-----------*/
			case 1:
				data = I2C_Slave_Receive();			// receive data byte
				if (data != -1)						// check whether STOP/REAPEATED START condition is received
				{
					sprintf(buffer, "%d", data);
					printString(buffer);
				}
				else
				{
					data = 0;
				}
				break;
				/*----------End for Slave Receiver Mode-----------*/
			default:
				break;
		}
    }	
}

