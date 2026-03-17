/*
	Serial Communication: USART
 */ 
#define F_CPU 12000000UL				// Clock CPU: 12MHz
#include <avr/io.h>
#include <stdio.h>
#include <util/delay.h>

#define BAUD_RATE	9600								// Baud: 9600bps
#define UBRR_VAL ((F_CPU/(BAUD_RATE*8UL)) - 1)			// Register USART Baud Rate Register: UBBR, mode: Asynchronous double speed mode

// USART initialization
void USART_init(void)
{
	// UCSR0A != (1 << U2X0);
	// Double speed mode
	UCSR0A = 0b00000010;
	
	// UCSR0B = 0b00011000, disable interrupt
	// UCSR0B = (1 << RXEN0) | (1 << TXEN0);	
	UCSR0B = 0b00011000;
	
	// UCSR0C = 0b00000110
	// bit 7:6 = 0 -> Asyn mode
	// bit 5:4 = 0 -> No parity bit
	// bit 3 = 0 -> Stop bit: 1 bit
	// bit 2:1 = 1 (UCSR0C) combined bit 2 = 0 (UCSR0B) -> data bit: 8 bit
	// UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);
	UCSR0C = 0b00000110;
	
	// Baud rate 9600 bps, assuming 12MHz Clock
	UBRR0H = (uint8_t)(UBRR_VAL >> 8);
	UBRR0L = (uint8_t)UBRR_VAL;
}

// Transmit data
void USART_Tx(uint8_t ch)
{
	// Wait until UDRE = 0 (i.e: Data Register isn't empty)
	while ((UCSR0A & (1 << UDRE0)) == 0x00){;}
	
	// USART Data Register store data
	UDR0 = ch;
}

// Receive data
uint8_t USART_Rx(void)
{
	/* Wait for data to be received */
	// RXC: Bit 1 neu co du lieu chua duoc
	while(!(UCSR0A & (1 << RXC0)));
	
	// Receive data from USART Data Register
	return UDR0;
}

// Send a string
void USART_SendString(char* str)
{
	uint8_t i = 0;
	while(str[i] != 0)
	{
		USART_Tx(str[i]);
		++i;
		_delay_ms(1000);
	}
}

int main(void)
{
	unsigned char c = '5';
    USART_init();							// Initalize USART
	USART_SendString("Echo Test");
    while(1)
    {
		// Receive a character
		c = USART_Rx();
		_delay_ms(1000);
		// Transmit received character
		USART_Tx(c);
		_delay_ms(1000);
    }
}

