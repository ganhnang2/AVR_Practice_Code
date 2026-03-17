/* 
	progmemdemo3.c
 */ 

#define F_CPU		12000000UL			// Clock CPU: 12MHz
#include <avr/io.h>
#include <avr/pgmspace.h>
#include "USART_RS232.h"
#include <util/delay.h>

/*	Strings are stored in FLASH memory */
const char menuStr0[] PROGMEM = "Hello World!\r\n";
const char menuStr1[] PROGMEM = "My name is Nguyen Hai Nam\r\n";
const char menuStr2[] PROGMEM = "I have studied at UET\r\n";

/*	An pointer array store these pointers to menuStr[i] and is also gotten to FLASH to save RAM memory
	-> It will take the code useable and easily modify	*/
const char* const strPointers[] PROGMEM = { menuStr0, menuStr1, menuStr2 };

void displayDataInProgmem(uint8_t index)
{
	/*	Assign pointer which points to string strPointers[index] into p	
		-> At this time, we have had starting address of string strPointers[index]	*/
	char* p = (char*) pgm_read_word(&strPointers[index]);
	
	char data;
	while((data = pgm_read_byte(p)))		// Fetch data at the selected address
	{
		transmitByte(data);		// transmit data
		p++;					// increment the address
		_delay_ms(100);
	}
}	

int main(void)
{
	uint8_t i;
	initUSART();
    while (1) 
    {
		for (i = 0; i < 3; i++)
			displayDataInProgmem(i);
		_delay_ms(500);
    }
}

