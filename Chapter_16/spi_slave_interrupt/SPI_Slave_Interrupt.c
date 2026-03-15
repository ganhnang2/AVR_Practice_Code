/*
	SPI_Slave_Interrupt.c
 */ 

#include "SPI_Slave_Interrupt.h"

volatile uint8_t receivedData;
volatile uint8_t transmittedData;

/*	Initialize SPI protocol	*/
void SPI_Init()									/* SPI Initialize function */
{
	SPI_DDR &= ~((1 << MOSI)| (1 << SCK) | (1 << SS));		/* Make MOSI, SCK, SS pin direction as input pins */
	SPI_PORT |= (1 << MISO);								/* Make MISO pin as output pin */
	SPI_PORT |= (1 << SS);									/* Enable resistor pull up	*/
	SPCR = (1<<SPE) | (1 << SPIE);							/* Enable SPI in slave mode */
}

/*	
	NOTE: Transfer data with master by using interrupt-driven technique
*/

/*	Transmit data in Master mode	*/
char SPI_Transmit(char data)					/* SPI transmit data function */
{
	return transmittedData;
}

/*	Receive data in Master mode	*/
char SPI_Receive()								/* SPI Receive data function */
{
	return receivedData;
}

/*	Transfer data with Master	*/ 
ISR(SPI_STC_vect)
{
	receivedData = SPDR;				// receive data from master
	
	SPDR = 	transmittedData;			// transmit data to master
}
