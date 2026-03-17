/*
	loggingThermometer.c
	
	Memory in EEPROM of IC 25LC256:
	+) Address 0x0000:1: store the address which the newest temperature is stored
	+) Address 0x0002:3: store the time interval between two consecutive sampling events
	+) Address 0x0004  : from this address, temperature value is sampled and transmitted on EEPROM
	
 */ 

#define F_CPU		120000000UL			// Clock CPU: 12MHz
#include <avr/io.h>
#include <util/delay.h>
#include "USART_RS232.h"
#include "25LC256.h"
#include "TWI_Master.h"

#define LED_DDR			DDRB
#define LED_PORT		PORTB
#define LED0			PORTB0

/*-----------------Define parameters in LM75------------------*/
#define LM75_ADDRESS			0b01001000
#define LM75_TEMP_REGISTER		0b00000000
#define	LM75_CONFIG_REGISTER	0b00000001
#define LM75_THYST_REGISTER		0b00000010
#define LM75_TSET_REGISTER		0b00000011

/*--------------Define pointers to store parameters in EEPROM---------------*/
#define CURRENT_LOCATION_POINTER		0			// where to store a pointer to the current reading in EEPROM 
#define SECOND_POINTER					2			// store seconds-delay value here
#define MEMORY_START					4			// where to start logging temperature values
#define MENU_DELAY						5			// seconds to wait before bypassing main menu

void printTemperature(uint8_t temperature)
{
	printByte(temperature >> 1);	
	if (temperature & 1)					// LSB indicates the fractional part after the decimal point 
		printString(".5\r\n");
	else
		printString(".0\r\n");
}

int main(void)
{
    uint16_t secondDelay;
	uint16_t currentMemoryLocation;
	uint16_t i;
	uint8_t receivedData;
	uint8_t tempHighByte, tempLowByte;
	uint8_t temperatureValue;
	uint8_t enterMenu;
	
	/*------------------Inits----------------*/
	SPI_Init();
	initUSART();
	I2C_Master_Init();
	LED_DDR |= (1 << LED0);
	
	// read seconds-delay value
	secondDelay = EEPROM_Read_Word(SECOND_POINTER);
	
	printString("*** Press [m] within ");
	printByte(MENU_DELAY);
	printString(" seconds to enter menu. ***\r\n ");
	
	printString("Enter an character: ");
	receivedData = receiveByte();
	transmitByte(receivedData);
	
	// if user don't enter character m, enterMenu = 0
	if (receivedData == 'm')
		enterMenu = 1;
	else
		enterMenu = 0;
	
	/*--------------Menu event-----------------*/
	while(enterMenu)
	{
		printString("\r\n====[  Logging Thermometer ]====\r\n  ");
		currentMemoryLocation = EEPROM_Read_Word(CURRENT_LOCATION_POINTER);			// read the address which stores the last temperature
		printWord(currentMemoryLocation - MEMORY_START);							// the number of data is stored in EEPROM
		printString(" readings in log.\r\n  ");
		printWord(secondDelay);
		printString(" seconds between readings.\r\n");
		printString(" [<] to shorten sample delay time\r\n");
		printString(" [>] to increase sample delay time\r\n");
		printString(" [?] to reset delay time to 60 sec\r\n");
		printString(" [d] to print out log over serial\r\n");
		printString(" [e] to erase memory\r\n");
		printString(" [s] to start logging\r\n\r\n");
		
		// enter an character
		printString("Enter an character: ");
		switch(receiveByte())
		{
			printString("\r\n");
			case 'd':
				for (i = MEMORY_START; i < currentMemoryLocation; i++)
				{
					printString("Log ");
					printByte(i);
					printString(":    ");
					temperatureValue = EEPROM_Read_Byte(i);
					printTemperature(temperatureValue);
					_delay_ms(20);
				}
				break;
			case 'e':
				EEPROM_Clear_All();
				EEPROM_Write_Word(CURRENT_LOCATION_POINTER, MEMORY_START);
				EEPROM_Write_Word(SECOND_POINTER, secondDelay);
				_delay_ms(100);
				break;
			case '?':
				secondDelay = 60;
				EEPROM_Write_Word(SECOND_POINTER, secondDelay);
				break;
			case '>':
				if (secondDelay < 65000)
				{
					secondDelay += 5;
					EEPROM_Write_Word(SECOND_POINTER, secondDelay);
				}
				break;
			case '<':
				if (secondDelay >= 10) {
				secondDelay -= 5;
				EEPROM_Write_Word(SECOND_POINTER, secondDelay);
				}
				break;
			case 's':
				printString("Logging data from LM75 to 25LM256!\r\n");
				enterMenu = 0;
				break;
			default:
				printString("Error! Please enter again.\r\n");
				break;
		}
		
		// display message after an operation is complete
		printString("DONE!\r\n");
	}
	
	/*----------Loop event----------*/
    while (1) 
    {
		currentMemoryLocation = EEPROM_Read_Word(CURRENT_LOCATION_POINTER);
		
		/*	Receive temperature value from IC LM75	*/
		I2C_Start();							// START condition
		I2C_Master_SLA_W(LM75_ADDRESS);			// Write operation to pointer register in LM75
		I2C_Write(LM75_TEMP_REGISTER);			// Select TEMP register
		I2C_Repeated_Start();					// REAPEATED START condition
		I2C_Master_SLA_R(LM75_ADDRESS);			// Read operation
		tempHighByte = I2C_Read_ACK();			// Read 2 data bytes from IC
		tempLowByte = I2C_Read_NACK();
		I2C_Stop();								// STOP condition
		
		// convert 9-bit to 8-bit to store in EEPROM
		temperatureValue = (tempHighByte << 1) | (tempLowByte >> 7);
		printString("Write temperature value into EEPROM: ");
		printTemperature(temperatureValue);
		
		// save the new temperature value 
		EEPROM_Write_Byte(currentMemoryLocation, temperatureValue);
		
		// move on to next location and record new position
		// if not already at the end of memory
		if (currentMemoryLocation < EEPROM_ADDRESS_BYTES_MAX)
		{
			currentMemoryLocation++;
			EEPROM_Write_Word(CURRENT_LOCATION_POINTER, currentMemoryLocation);
		}
		
		// blink LED to indicate data logging successfully
		for (i = 0; i < secondDelay; i++)
		{
			LED_PORT ^= (1 << LED0);
			_delay_ms(100);
		}

    }
}

