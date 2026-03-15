
#include <avr/io.h>
#include "SPI_Slave.h"

/*	Initialize SPI protocol	*/
void SPI_Init()									/* SPI Initialize function */
{
	SPI_DDR &= ~((1 << MOSI)| (1 << SCK) | (1 << SS));		/* Make MOSI, SCK, SS pin direction as input pins */
	SPI_PORT |= (1 << MISO);								/* Make MISO pin as output pin */
	SPI_PORT |= (1 << SS);									/* Enable resistor pull up	*/
	SPCR = (1<<SPE);										/* Enable SPI in slave mode */
}

/*	NOTE: The SPIF bit is cleared with following steps: 
	Step 1: Reading the SPI status register with SPIF set
	Step 2: Accessing (Write or Read) the SPI data register (SPDR).
*/

/*	Transmit data in Master mode	*/
char SPI_Transmit(char data)					/* SPI transmit data function */
{
	SPDR = data;								/* Write data to SPI data register */
	while(!(SPSR & (1 << SPIF)));				/* Wait till transmission complete */
	return(SPDR);								/* return received data */
}

/*	Receive data in Master mode	*/
char SPI_Receive()								/* SPI Receive data function */
{
	while(!(SPSR & (1 << SPIF)));				/* Wait till reception complete */
	return(SPDR);								/* return received data */
}