/*
 * SPI_Slave.h
 *
 * Created: 1/22/2026 3:33:55 PM
 *  Author: ASUS
 */ 


#ifndef SPI_SLAVE_H_
#define SPI_SLAVE_H_

/*	Define pins for SPI protocol	*/
#define SPI_DDR		DDRB
#define SPI_PORT	PORTB
#define SPI_PIN		PINB
#define SS			PORTB2			// SS (Slave Select)
#define MOSI		PORTB3			// MOSI (Master In Slave Out)
#define MISO		PORTB4			// MISO (Master Out Slave In)
#define SCK			PORTB5			// SCK (Serial Clock)

void SPI_Init(void);				// Initiate SPI mode
char SPI_Transmit(char);			// Transmit data to Slave through MOSI line
char SPI_Receive(void);				// Receive data from Slave through MISO line



#endif /* SPI_SLAVE_H_ */