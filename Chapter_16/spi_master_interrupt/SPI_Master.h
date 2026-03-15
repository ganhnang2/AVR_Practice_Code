/*
	SPI_Master.h
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>

#ifndef SPI_MASTER_H_
#define SPI_MASTER_H_

/*	Define pins for SPI protocol	*/
#define SPI_DDR		DDRB
#define SPI_PORT	PORTB
#define SPI_PIN		PINB
#define MOSI		PORTB3			// MOSI (Master In Slave Out)
#define MISO		PORTB4			// MISO (Master Out Slave In)
#define SCK			PORTB5			// SCK (Serial Clock)

#define NUMBER_OF_SLAVES	3		// has 3 slaves 

/*	Define SS pins	*/
#define SS_DDR		DDRD
#define SS_PORT		PORTD
#define Slave(i)	i				// PORTDi (i = 0 ... 7)		

void SPI_Init(void);				// Initiate SPI mode
void SPI_Transmit(uint8_t, char);	// Transmit data to Slave through MOSI line
uint8_t SPI_Receive(void);			// Receive data from Slave through MISO line

#endif /* SPI_MASTER_H_ */