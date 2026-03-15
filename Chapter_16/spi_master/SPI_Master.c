/*
	SPI_Master.c
 */ 

#include <avr/io.h>
#include "SPI_Master.h"

volatile uint8_t dataReceived;

/*	Initialize SPI protocol	*/
void SPI_Init(void)
{
	SPI_DDR |= (1 << MOSI) | (1 << SCK);			// set MOSI and SCK pins as output
	SPI_DDR &= ~(1 << MISO);						// configure MISO as input
	SS_DDR |= (1 << Slave(0)) | (1 << Slave(1)) | (1 << Slave(2));	// set SS pins as output
	
	// Slave(i) is equivalent to PORTDi
	SS_PORT |= (1 << Slave(0)) | (1 << Slave(1)) | (1 << Slave(2));	// SS = HIGH -> the SPI is passive		
	SPI_PORT |= (1 << SS);				// enable SS HIGH
								
	// Bit 6 (SPE) = 1 -> Enable SPI
	// bit 5 (DORD) = 0 -> MSB transmitted first
	// Bit 4 (MSTR) = 1 -> Master mode
	// Bit 1:0 = 1 -> SCK = fosc/16 
	SPCR |= (1 << SPE) | (1 << MSTR) | (1 << SPR0);				
}

/*	NOTE: The SPIF bit is cleared with following steps: 
	Step 1: Reading the SPI status register with SPIF set
	Step 2: Accessing (Write or Read) the SPI data register (SPDR).
*/

/*	Transmit data in Master mode
	slaveID: Choose a specific slave to transmit
	data: Data need to be transmitted			*/
void SPI_Transmit(uint8_t slaveID, char data)
{
	SS_PORT &= ~(1 << slaveID);				// SS = LOW -> the SPI is active
	SPDR = data;							// Write data to SPDR		
	
	/*	Reading SPIF in SPI status register	*/
	while (!(SPSR & (1 << SPIF)));			// Wait till transmission complete
	
	/*	Accessing SPDR	*/
	dataReceived = SPDR;			
	/*	At this moment, bit SPIF is cleared -> SPIF = 0	*/
		
	SS_PORT |= (1 << slaveID);				// SS = HIGH -> the SPI is passive
}

/*	Receive data in Master mode	*/
uint8_t SPI_Receive(void)
{
	SPDR = 0x00;							// Since, writing to SPDR to generate SCK for transmission, 
											// write dummy data in the SPDR register
	while (!(SPSR & (1 << SPIF)));			// Wait till reception complete (Step 1 to clear SPIF)
	return (SPDR);							// Return received data (Step 2 to clear SPIF)
}