/*
	lcd16x02.c
 */ 
 
#ifndef F_CPU
#define F_CPU	12000000UL			// Clock CPU: 12MHz
#endif

#include <avr/io.h>
#include <util/delay.h>
#include "lcd16x02.h"

/*	Helper fuction lcdCommand()	*/
void lcdCommand(uint8_t data)
{
	COMMAND_PORT &= ~((1 << RS) | (1 << RW));			// RS = 0, RW = 0
	
	/* High nibble of uint8_t data */
	DATA_PORT = (DATA_PORT & 0x0F) | (data & 0xF0);		// high nibble of uint8_t data
	COMMAND_PORT |= (1 << EN);							// set EN HIGH
	_delay_us(450);
	COMMAND_PORT &= ~(1 << EN);							// falling edge on EN: HIGH -> LOW to enable signal
	_delay_us(10);
	
	/* Low nibble of uint8_t data */
	DATA_PORT = (DATA_PORT & 0x0F) | (data << 4);		// low nibble of uint8_t data
	COMMAND_PORT |= (1 << EN);							// set EN HIGH
	_delay_us(450);
	COMMAND_PORT &= ~(1 << EN);							// falling edge on EN: HIGH -> LOW to enable signal
	_delay_us(10);
}

/*	Initialize LCD16x02	*/
void lcdBegin(void)
{
	DATA_DDR |= (1 << D4) | (1 << D5) | (1 << D6) | (1 << D7);		// set as output
	COMMAND_DDR |= (1 << EN) | (1 << RS) | (1 << RW);
	
	// After power on, setup 8-bit mode to synchronize with lcd16x02
	// -> to be stable at the beginning of initialization.
	lcdCommand(0x33);
	_delay_us(50);
	
	// set 4-bit mode
	lcdCommand(0x32);
	_delay_us(50);
	
	// DB7:0 = 0b00101000 -> 4 bit mode, 2-line display and 5x8 dots format display
	lcdCommand(0x28);
	_delay_us(50);
	
	// DB7:0 = 0b00001100 -> turn on entire display and cursor; don't blink cursor
	lcdCommand(0x0E);
	_delay_us(50);
	
	// DB7:0 = 0b00000001 -> clear display
	lcdCommand(0x01);
	_delay_ms(2);
	
	// Entry mode set -> cursor/ blink moves to right automatically
	lcdCommand(0x06);
}

/*	Clear LCD and set the cursor original position	*/
void clearDisplay()
{
	lcdCommand(0x01);		// clear display
	_delay_ms(2);
	lcdCommand(0x80);		// return home
}

/*	Move cursor to specific position	*/
void setCursor(uint8_t row, uint8_t col)
{
	if (row == 0 && col < 16)
		lcdCommand((0x0F & col) | 0x80);					// first row
	else if (row == 1 && col < 16)
		lcdCommand((0x0F & col) | (0x80 | 0x40));			// second row
}

void printCharacter(uint8_t data)
{
	
	/*	Write data to RAM	*/
	COMMAND_PORT &= ~(1 << RW);							// RS = 1, RW = 0
	COMMAND_PORT |= (1 << RS);
	
	DATA_PORT = (DATA_PORT & 0x0F) | (data & 0xF0);		// high nibble of uint8_t data
	COMMAND_PORT |= (1 << EN);							// set EN HIGH
	_delay_us(450);
	COMMAND_PORT &= ~(1 << EN);							// falling edge on EN: HIGH -> LOW to enable signal
	_delay_us(10);
	DATA_PORT = (DATA_PORT & 0x0F) | (data << 4);		// low nibble of uint8_t data
	COMMAND_PORT |= (1 << EN);							// set EN HIGH
	_delay_us(450);
	COMMAND_PORT &= ~(1 << EN);							// falling edge on EN: HIGH -> LOW to enable signal
	_delay_us(10);
}

/*	Display string on LCD16x02	*/
void printString(const char* str)
{
	uint8_t i = 0;
	while (str[i] != '\0')			// loop until character is NULL
	{
		printCharacter(str[i]);
		++i;
	}
}

/*	Display string at any positions on LCD16x02	*/
void printStringAtAnyPosition(uint8_t row, uint8_t col, const char* str)
{
	setCursor(row, col);
	printString(str);
}

/*	Shift all the display to the right	*/
void scrollDisplay()
{
	// Entry mode set -> shift of entire display right
	lcdCommand(0x1C);
	_delay_ms(100);
}


