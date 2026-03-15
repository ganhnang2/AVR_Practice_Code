/*
	I2C_Interface_LM75.c
	In the sketch, i use I2C protocol to interface thermometer LM75. 
	
	## Hardware
	---------------------------------------------------------
	|		ATmega328P pin		|			LM75 pin		|
	---------------------------------------------------------
	|			SDA				|			SDA				|
	---------------------------------------------------------
	|			SCL				|			SCL				|
	---------------------------------------------------------
	|			GND				|			A0				|
	---------------------------------------------------------
	|			GND				|			A1				|
	---------------------------------------------------------
	|			GND				|			A2				|
	---------------------------------------------------------
	
	## Firmware
	Programming to read data from IC LM75.
	NOTE: 
	1. A2:0 = 0 -> LM75 address (7-bit) = 0b01001000
	2. The value of Temperature in LM75 is in 2's-complement binary format (9-bit).  In this representation,
	- LSB indicates the fractional part after the decimal point 
	- the remaining 8-bit represent the integer part
	
	Step to read TEMP from LM75 (Look up the datasheet):
	1. Send START condition
	2. Write device Write address (SLA+W) and check for ACK.
	3. After receiving ACK, value in Pointer Register (POINT) is transmitted
	4. Generate REAPEATED START on SDA
	5. Write device Read address (SLA+R) and check for ACK.
	6. After checking ACK, receiving Most Significant Data Byte and Least Significant Data Byte from LM75
	7. Send NACK for not getting data
	8. Finally, AVR send STOP condition
	
	Source: Nguyen Hai Nam
 */ 

#define F_CPU		12000000UL
#include <avr/io.h>
#include <util/delay.h>
#include "TWI_Master.h"
#include "USART_RS232.h"

#define LM75_ADDRESS			0b01001000
#define LM75_TEMP_REGISTER		0b00000000
#define	LM75_CONFIG_REGISTER	0b00000001
#define LM75_THYST_REGISTER		0b00000010
#define LM75_TSET_REGISTER		0b00000011

int main(void)
{
	uint8_t tempHighByte, tempLowByte;
    /*-------------------Inits------------------*/
	I2C_Master_Init();
	initUSART();
	
	printString("\r\n====  I2C Thermometer  ====\r\n");
	
	/*---------------Loop event--------------*/
    while (1)		
    {
		/*	Receive data from IC LM75	*/
		I2C_Start();							// START condition
		I2C_Master_SLA_W(LM75_ADDRESS);			// Write operation to pointer register in LM75
		I2C_Write(LM75_TEMP_REGISTER);			// Select TEMP register
		I2C_Repeated_Start();					// REAPEATED START condition
		I2C_Master_SLA_R(LM75_ADDRESS);			// Read operation
		tempHighByte = I2C_Read_ACK();			// Read 2 data bytes from IC
		tempLowByte = I2C_Read_NACK();	
		I2C_Stop();								// STOP condition
		
		// Print it out nicely over serial for now...
		printByte(tempHighByte);
		if (tempLowByte & (1 << 7))				// Check fraction part
		{
			printString(".5\r\n");
		}
		else 
		{
			printString(".0\r\n");
		}
		
		_delay_ms(1000);
    }
}

