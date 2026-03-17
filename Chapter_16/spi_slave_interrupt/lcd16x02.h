/*
	lcd16x02.h
	Programming to display on LCD16x02 from the scratch.
	
	## Hardware
	|	AVR pin	|	LCD pin	|
	|-----------|-----------|
	|	PORTC0	|	RS		|
	|	PORTC1	|	R/W		|
	|	PORTC2	|	EN		|
	|	PORTB4	|	D4		|
	|	PORTB5	|	D5		|
	|	PORTB6	|	D6		|
	|	PORTB7	|	D7		|
	
	## Firmware
	First, I will initialize the LCD in 4-bit mode and define certain modes for consistent use 
	across this sketch
	Then, write a helper function lcdCommand(), this function takes an argument (8-bit) as an
	instruction, which MPU (LCD16x02) interprets to carry out the desired functionality.
	Finally, I have implemented basic fuctions as printString(), clearDisplay(), ...
	
	NOTE: When lcd16x02 operates at 4-bus mode, it needs to transfer 4-bit data twice:
	first from DB7 to DB4, then from DB3 to DB0.
	
	Source: Nguyen Hai Nam
 */ 


#ifndef LCD16X02_H_
#define LCD16X02_H_

/*	Define device pins	*/
#define DATA_PORT		PORTD
#define DATA_DDR		DDRD
#define D4				PORTD4			// DB4:DB7 = high nibble
#define D5				PORTD5
#define D6				PORTD6
#define D7				PORTD7

#define COMMAND_DDR		DDRC
#define COMMAND_PORT	PORTC
#define RS				PORTC0
#define RW				PORTC1
#define EN				PORTC2

/*	Prototype function	*/
void lcdBegin(void);
void lcdCommand(uint8_t);
void clearDisplay();
void setCursor(uint8_t, uint8_t);
void printCharacter(uint8_t);
void printString(const char*);
void printStringAtAnyPosition(uint8_t, uint8_t, const char*);
void scrollDisplay();



#endif /* LCD16X02_H_ */