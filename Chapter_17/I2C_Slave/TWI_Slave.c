/*
`	TWI_Slave.c
 */ 

#include "TWI_Slave.h"

// I2C slave initialize function with Slave address 
void I2C_Slave_Init(uint8_t address)
{
	TWAR = (address << 1) | 1;							// load the address into TWAR and enable general call
	TWCR = (1 << TWEN) | (1 << TWINT) | (1 << TWEA);	// enable TWI, enable ack generation 
}	

// I2C slave listen function 
int8_t I2C_Slave_Listen()
{
	uint8_t status;
	/*	Slave has listened for responding to SLA+R/W transmitted from master */
	while (1)
	{
		while(!(TWCR & (1 << TWINT)));			// Wait to be addressed 
		status = TWSR & 0xF8;
		if (status == 0x60 || status == 0x68)	// own SLA+W received and ACK returned
			return 0;
		if (status == 0xA8 || status == 0xB0)	// own SLA+R received and ACK returned
			return 1;
		if (status == 0x70 || status == 0x78)	// general call received and ACK returned
			return 2;
		else
			continue;
	}
}					

// I2C slave transmit function
int8_t I2C_Slave_Transmit(char data)
{
	uint8_t status;
	TWDR = data;
	TWCR = (1 << TWEN) | (1 << TWINT) | (1 << TWEA);
	while(!(TWCR & (1 << TWINT)));
	status = TWSR & 0xF8;
	if (status == 0xA0)							// check whether STOP/REAPEATED START
	{
		TWCR |= (1 << TWINT);					// clear the flag
		return -1;
	}
	
	if (status == 0xB8)							// check data transmitted and ACK returned from master
		return 0;
	if (status == 0xC0)							// check data transmitted and NACK returned from master
	{
		TWCR |= (1 << TWINT);					// clear the flag
		return -2;
	}
	if (status == 0xC8)							// special case: don't have any data but master still require send 
		return -3;
	else
		return -4;
}
			
// I2C slave transmit function
char I2C_Slave_Receive()
{
	uint8_t status;
	TWCR = (1 << TWEA) | (1 << TWINT) | (1 << TWEN);
	while(!(TWCR & (1 << TWINT)));
	status = TWSR & 0xF8;
	
	if (status == 0x80 || status == 0x90)		// check for data received and ACK returned
		return TWDR;
	if (status == 0x88 || status == 0x98)		// check for data received and NACK returned
		return TWDR;	
	if (status == 0xA0)							// check whether STOP/REAPEATED START
	{
		TWCR |= (1 << TWINT);					// clear the flag
		return -1;
	}
	else
		return -2;
		
}					