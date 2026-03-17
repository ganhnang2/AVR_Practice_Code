/*
	TWI_Slave_ver2.h
	Using interrupt-driven technique to programming in slave mode
	
	SOURCE: Nguyen Hai Nam
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>

#ifndef TWI_SLAVE_VER2_H_
#define TWI_SLAVE_VER2_H_

#define TOTAL_RECEIVED_DATA			10

/*------------Variable---------------*/
volatile uint8_t transmittedData = 0;				// -1: don't transmit, others: data is sent to Master
volatile uint8_t slaveMode = 1;						// 0: Transmitter, 1: Receiver	
volatile int8_t receivedData = 0;					// -1: stop receiving data, others: received data from master 

volatile uint8_t dataBuffer[TOTAL_RECEIVED_DATA];	// transmit data to master
volatile uint8_t indexData = 0;

/**************************FUNCTION*******************************/
// I2C slave initialize function with Slave address
void I2C_Slave_Init(uint8_t address)
{
	TWAR = (address << 1) | 1;										// load the address into TWAR and enable general call
	TWCR = (1 << TWEN) | (1 << TWINT) | (1 << TWEA) | (1 << TWIE);	// enable TWI, enable ACK generation
}

// check whether data is sent successfully
uint8_t I2C_Slave_Transmit(uint8_t data[], uint8_t length)
{
	for (uint8_t i = 0; i < length; i++)
	{
		dataBuffer[i] = data[i];
	}
	return transmittedData;
}

int8_t I2C_Transmit_Status()
{
	return transmittedData;		// -1: don't transmit, others: data is sent to Master
}

// return received data
int8_t I2C_Slave_Receive()
{
	return receivedData;			
}

// Slave mode: Transmitter/ Receiver
uint8_t I2C_Slave_Mode()
{
	return slaveMode;
}

/**************************END FOR FUNCTION*********************************/

/**************************TWI ISR****************************/
ISR(TWI_vect)
{
	uint8_t status;
	status = TWSR & 0xF8;
	switch(status)
	{
		/*-------------Slave Receiver Mode---------------*/
		// SLA+W received and ACK returned
		case 0x60: case 0x68:						
			slaveMode = 1;
			TWCR = (1 << TWEN) | (1 << TWINT) | (1 << TWEA) | (1 << TWIE);		// clear TWINT flag and set ACK pulse
			break;
		// receive data byte
		case 0x80: case 0x90: case 0x88: case 0x98:	
			receivedData = TWDR;
			TWCR = (1 << TWEN) | (1 << TWINT) | (1 << TWEA) | (1 << TWIE);		// clear TWINT flag and set ACK pulse
			break;
		// STOP/REAPEATED START condition has been received
		case 0xA0:
			receivedData = -1;			
			TWCR = (1 << TWEN) | (1 << TWINT) | (1 << TWEA) | (1 << TWIE);
			break;
		/*------------End for Slave Receiver Mode---------------*/
		
		/*-------------Slave Transmitter Mode---------------*/
		case 0xA8: case 0xB0:			// SLA+R received and ACK returned
			indexData = 0;
			slaveMode = 0;
			TWDR = dataBuffer[indexData];
			transmittedData = dataBuffer[indexData];
			indexData++;
			TWCR = (1 << TWEN) | (1 << TWINT) | (1 << TWEA) | (1 << TWIE);
			break;
		case 0xB8:										// transmit data byte
			if (indexData < TOTAL_RECEIVED_DATA)		// check if data has finished 
			{
				TWDR = dataBuffer[indexData];
				transmittedData = dataBuffer[indexData];
				indexData++;
			}	
			// if data is empty but master has still received, send dummy data (0x00)
			else		
				TWDR = 0x00;
			TWCR = (1 << TWEN) | (1 << TWINT) | (1 << TWEA) | (1 << TWIE);
			break;
		case 0xC0:						// finish transmitting data byte
			transmittedData = -1;
			TWCR |= (1 << TWINT) | (1 << TWEN) | (1 << TWIE);		// clear the flag
			break;
		/*------------End for Transmitter Mode---------------*/
		default:
			transmittedData = -1;
			receivedData = -1;	
			TWCR = (1 << TWEN) | (1 << TWINT) | (1 << TWIE);
			break;
	}
}
/**************************END FOR TWI ISR*********************************/

#endif /* TWI_SLAVE_VER2_H_ */