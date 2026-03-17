/*
	SPI_Slave_Interrupt.h

 */ 

#include <avr/interrupt.h>
#include <avr/io.h>

#ifndef SPI_SLAVE_INTERRUPT_H_
#define SPI_SLAVE_INTERRUPT_H_

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




#endif /* SPI_SLAVE_INTERRUPT_H_ */