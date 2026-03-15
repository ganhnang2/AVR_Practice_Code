/*
	TWI_Slave.h

 */ 

#include <avr/io.h>
#include <avr/interrupt.h>

#ifndef TWI_SLAVE_H_
#define TWI_SLAVE_H_

#ifdef F_CPU
#undef F_CPU
#endif

#define F_CPU	12000000UL			// Clock CPU: 12MHz

/*	Prototype function	*/
void I2C_Slave_Init(uint8_t address);			// I2C slave initialize function with Slave address 
int8_t I2C_Slave_Listen();						// I2C slave listen function 
int8_t I2C_Slave_Transmit(char data);			// I2C slave transmit function 
char I2C_Slave_Receive();						// I2C slave receive function 		



#endif /* TWI_SLAVE_H_ */