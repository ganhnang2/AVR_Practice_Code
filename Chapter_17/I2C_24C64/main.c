/*
	I2C_24C64.c
	Store data into EEPROM AT24C64 by I2C protocol
	
	After accessing data in EEPROM, display value stored on LCD16x02.
 */ 

#define F_CPU		12000000UL			// Clock CPU: 12MHz
#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <avr/pgmspace.h>
#include "24C64.h"
#include "USART_RS232.h"

const char stringOfMenu[] PROGMEM = "[r] to read from memory\r\n\
	[w] to write byte to memory\r\n \
	[e] to write sequences byte to memory\r\n\r\n \
	Enter an character: ";

int main(void)
{
	char buffer[50];
	uint16_t address = 0;
	uint16_t startAddress, endAddress;
	volatile uint8_t data;
	uint8_t array[10] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    /*----------------Inits-----------------*/
	IC24C64_Init();
	initUSART();
	
	/*--------------Loop event-------------*/
   while (1)
   {
	   printString_PROGMEM(stringOfMenu);
	   data = receiveByte();
	   transmitByte(data);
	   printString("\r\n");
	   
	   switch(data)
	   {
			// receive data from selected addresses
			case 'r':
				printString("Enter an starting address: ");				// start address
				startAddress = getNumber16Bit();
				printString("\r\n");
				printString("Enter an ending address: ");				// end address
				endAddress = getNumber16Bit();
				printString("\r\n");
		   
				printString("Receive data from EEPROM\r\n");
				// display value on LCD
				for (address = startAddress; address <= endAddress; address++)
				{
					data = IC24C64_Read_One_Byte(address);
					sprintf(buffer, "Address %d:   ", address);
					printString(buffer);
					printByte(data);
					printString("\r\n");
					_delay_ms(500);
				}
				break;
			// transmit data into selected address
			case 'w':
				printString("Transmit data to EEPROM\r\n");
				printString("Enter an selected address: ");
				address = getNumber16Bit();
				printString("\r\n");
				printString("Enter value on selected address: ");
				data = getNumber();
				IC24C64_Write_One_Byte(data, address);
				sprintf(buffer, "Address %d:   ", address);
				printString(buffer);
				printByte(data);
				printString("\r\n");
				_delay_ms(500);
				break;
			case 'e':
				printString("Perform a sequential write of the numbers from 0 to 9\r\n");
				printString("Enter an starting address: ");				// start address
				startAddress = getNumber16Bit();
				printString("\r\n");
				printWord(startAddress);			
				printString("\r\n");
				IC24C64_Write_Sequence_Bytes(array, startAddress, 10);
				for (uint8_t i = 0; i < 10; i++)
				{
					data = IC24C64_Read_One_Byte(startAddress + i);
					sprintf(buffer, "Address %d:   ", startAddress + i);
					printByte(data);
					printString("\r\n");
					_delay_ms(500);
				}
				break;
			default:
				address = 0;
				startAddress = 0;
				endAddress = 0;
				printString("Error! Please enter again \r\n");
				break;
		}
	   
	   data = '\0';		// reset
   }
}

