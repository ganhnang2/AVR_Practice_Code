/*
	I2C_Master.c
	
	Programming steps in the master device
	1. Initialize I2C.
	2. Generate START condition.
	3. Write device Write address (SLA+W) and check for acknowledgement.
	4. After acknowledgement write data to slave device.
	5. Generate REPEATED START condition with SLA+R.
	6. Receive data from slave device.
 */ 

#define F_CPU		12000000UL		// Clock CPU: 12MHz
#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include "TWI_Master.h"
#include "lcd16x02.h"

#define SLAVE_ADDRESS		0x02	// slave address to transfer data
const uint8_t count = 10;
uint8_t receivedData = 0;
char buffer[10];
uint8_t i;

void transmittingData()
{
	printStringAtAnyPosition(0, 0, "Master Device");
	/*----------Master Transmitter Mode-----------*/
	printStringAtAnyPosition(1, 0, "Sending          ");
	I2C_Start();									// generate START condition
	I2C_Master_SLA_W(SLAVE_ADDRESS);				// send SLA+W to slave
	for (i = 0; i < count; i++)
	{
		sprintf(buffer, "%d", i);					// convert format to display data
		printStringAtAnyPosition(1, 13, buffer);
		I2C_Write(i);
		_delay_ms(500);
	}
	/*----------End for Master Transmitter Mode-----------*/
	I2C_Stop();			// Stop I2C
}

void receivingData()
{
	uint8_t dataBuffer[10];
	/*----------Master Receiver Mode-----------*/
	I2C_Start();									// generate REAPEATED START condition
	I2C_Master_SLA_R(SLAVE_ADDRESS);				// send SLA+R to slave
		
	printStringAtAnyPosition(0, 0, "Master Device");
	printStringAtAnyPosition(1, 0, "Receiving ");
	
	// delay 5 milliseconds to synchronize with the slave
	// If not delay, first data sent is dummy data (255)
	_delay_ms(5);
	
	for (uint8_t i = 0; i < count; i++)
	{
		if (i < count - 1)
			dataBuffer[i] = I2C_Read_ACK();
		else
			dataBuffer[i] = I2C_Read_NACK();
		sprintf(buffer, "%d", dataBuffer[i]);
		printStringAtAnyPosition(1, 13, buffer);
		_delay_ms(500);
	}
	
	/*----------End for Master Receiver Mode-----------*/
	I2C_Stop();			// Stop I2C
}

int main(void)
{
	/*---------------Inits--------------*/
	I2C_Master_Init();
	lcdBegin();
	
	/*------------Loop event------------*/
    while (1) 
    {
		transmittingData();
		_delay_ms(10);
		receivingData();
    }
}
