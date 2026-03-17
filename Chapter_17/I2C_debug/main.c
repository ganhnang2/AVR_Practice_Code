/*
 * I2C_debug.c
 *
 * Created: 1/30/2026 11:08:30 PM
 * Author : ASUS
 */ 

#define F_CPU		12000000UL		// Clock CPU: 12MHz
#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <util/twi.h>
#include "25LC256.h"
#include "lcd16x02.h"
#include "USART_RS232.h"

/*	Display data on LCD 
	ch: character indicate selected mode ('r': Read, 'w': Write)	*/
void displayData(uint8_t address, uint8_t data, char ch)
{
	char buffer[20];
	sprintf(buffer, "Address: %d", address);
	LCD_Print_String_At_Position(0 ,0, buffer);
	if (ch == 'r')
	{
		sprintf(buffer, "Receive: %d", data);
		LCD_Print_String_At_Position(1, 0, buffer);
	}
	else
	{
		sprintf(buffer, "Transmit: %d", data);
		LCD_Print_String_At_Position(1, 0, buffer);
	}
}

int main(void)
{
	uint16_t word = 1000;
	uint8_t address;
	uint8_t startAddress, endAddress;
	uint8_t receivedChar;
	uint8_t data;
	uint16_t dataWord;
    /*----------------Inits-----------------*/
	SPI_Init();
	LCD_Begin();
	initUSART();
	
	/*--------------Loop event-------------*/
    while (1) 
    {	
		printString(" [r] to read byte from memory\r\n");
		printString(" [t] to read word from memory\r\n");
		printString(" [w] to write byte to memory\r\n");
		printString(" [u] to write word to memory\r\n");
		printString("Enter an character: ");
		receivedChar = receiveByte();
		transmitByte(receivedChar);
		printString("\r\n");
		
		switch(receivedChar)
		{
			// receive data from selected addresses
			case 'r':	
				printString("Enter an starting address: ");				// start address
				startAddress = getNumber();
				printString("\r\n");
				printString("Enter an ending address: ");				// end address
				endAddress = getNumber();
				printString("\r\n");
				
				// display value on LCD
				for (address = startAddress; address <= endAddress; address++)			
				{
					data = EEPROM_Read_Byte(address);
					displayData(address, data, receivedChar);
					_delay_ms(500);
					LCD_Clear_Display();
				}
				break;
			// transmit data into selected address
			case 'w':
				printString("Enter an selected address: ");
				address = getNumber();
				printString("\r\n");
				printString("Enter value on selected address: ");
				data = getNumber();
				EEPROM_Write_Byte(address, data);
				displayData(address, data, receivedChar);
				_delay_ms(500);
				break;
			case 'u':
				printString("Enter an selected address: ");
				address = getNumber();
				printString("\r\n");
				EEPROM_Write_Word(address, word);
				displayData(address, (word >> 8), 'w');
				_delay_ms(500);
				displayData(address, word, 'w');
				_delay_ms(500);
				word++;
				break;
			case 't':
				printString("Enter an selected address: ");
				address = getNumber();
				dataWord = EEPROM_Read_Word(address);
				displayData(address, (dataWord >> 8), 'r');
				_delay_ms(500);
				displayData(address, dataWord, 'r');
				_delay_ms(500);
				break;
			default:
				address = 0;
				startAddress = 0;
				endAddress = 0;
				printString("Error, enter again!\r\n");
				LCD_Clear_Display();
				break;
		}
		
		receivedChar = '\0';		// reset
		LCD_Clear_Display();
    }
}

