
/*
	Các hàm ??n gi?n giúp truy?n thông n?i ti?p ho?t ??ng.

	L?u ý r?ng receiveByte() là hàm d?ng blocking — nó s? ng?i ch? mãi mãi cho ??n khi có 
	m?t byte d? li?u ??n. N?u b?n ?ang làm ?i?u gì ?ó thú v? h?n, b?n s? mu?n tri?n khai 
	ch?c n?ng này b?ng ng?t (interrupts).

	initUSART yêu c?u BAUDRATE ph?i ???c ??nh ngh?a ?? tính toán h? s? nhân t?c ?? truy?n (bit-rate multiplier). 
	Giá tr? 9600 là m?t m?c ??nh h?p lý.

	NOTE: Có th? không ho?t ??ng v?i vài chip AVR c? nh?: Mega8, Mega16, Mega32, Tiny2313, ...
*/

#include <avr/io.h>
#include "USART.h"
#include <util/setbaud.h>

// Kh?i t?o USART
void initUSART(void)
{
	// UBRR0H và UBRR0L ???c tính toán trong file <setbaud.h>
	UBRR0H = UBRRH_VALUE;
	UBRR0L = UBRRL_VALUE;
	
	// Ch? th? ti?n x? lý v? vi?c có s? d?ng ch? ?? double-speed hay không?
	#if USE_2X
	UCSR0A |= (1 << U2X0);
	#else
	UCSR0A &= ~(1 << U2X0);
	#endif

	// Kh?i t?o 2 thanh ghi UCSR0B và UCSR0C
	UCSR0B = (1 << RXEN0) | (1 << TXEN0);				// Receive, Transmit mode enable
	UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);				// 8 data bits, 1 stop bit, no parity bit
}

// Hàm truy?n d? li?u qua USART
void transmitByte(uint8_t data)
{
	// ??i ??n khi UDRE = 1 (hay là Data Register s?n sàng nh?n d? li?u)
	while ((UCSR0A & (1 << UDRE0)) == 0x00){;}
	
	// USART Data Register store data
	UDR0 = data;
}

// Hàm nh?n d? li?u qua USART
uint8_t receiveByte(void)
{
	/* ??i d? li?u ??n */
	// RXC: Bit 1 neu co du lieu chua duoc
	while(!(UCSR0A & (1 << RXC0)));
	
	// Nh?n d? li?u t? USART Data Register
	return UDR0;
}

/* 	Hàm ti?n ích ?? truy?n toàn b? chu?i ký t? t? RAM	*/
void printString(const char myString[])
{
	uint8_t i = 0;
	while (myString[i] != '\0')
	{
		transmitByte(myString[i]);
		++i;
	}
	
}

/* 	??nh ngh?a m?t bi?n char[] truy?n vào hàm, bi?n này s? l?u tr?
	t?t c? nh?ng gì b?n ?ã truy?n qua c?ng Serial	*/
void readString(char myString[], uint8_t maxLength)
{
	char response;
	uint8_t i = 0;
	// i < maxLength - 1: ?? ký t? cu?i là ký t? NULL 
	while (i < (maxLength - 1)) 
	{                   
		response = receiveByte();		// Nh?n m?t ký t? 
		transmitByte(response);         // Ph?n h?i l?i lên terminal ?? xác nh?n vi?c ?ã ??c thành công?
		if (response == '\r') 			// N?u nh?p ký t? return ('\r') thì d?ng vi?c ??c                  
			break;
		else 
		{
		myString[i] = response;         // Thêm m?t ký t? vào chu?i
		i++;
		}
	}
	myString[i] = 0;                    // Ký t? NULL
}

/*	In byte thành m?t s? ASCII 3-digits t??ng ?ng */
void printByte(uint8_t byte)
{
	transmitByte('0' + (byte / 100));					// Hàng tr?m
	transmitByte('0' + ((byte / 10) % 10));				// Hàng ch?c
	transmitByte('0' + byte % 10);						// Hàng ??n v?
}

/*	In ra m?t word (16-bits) thành m?t s? ASCII 5-digit t??ng ?ng 	*/
void printWord(uint16_t word)
{
	transmitByte('0' + (word / 10000));                 // Hàng ch?c nghìn
	transmitByte('0' + ((word / 1000) % 10));           // Hàng nghìn
	transmitByte('0' + ((word / 100) % 10));            // Hàng tr?m
	transmitByte('0' + ((word / 10) % 10));             // Hàng ch?c
	transmitByte('0' + (word % 10));                    // Hàng ??n v?
}

/* 	In ra m?t byte d??i d?ng h? nh? phân 0s và 1s	*/
void printBinaryByte(uint8_t byte)
{
	// MSB -> LSB
	for (uint8_t i = 8; i > 0; i--)
		transmitByte((byte & (1 << (i - 1)) ? '1' : '0'));
}

/* 	In ra m?t byte d??i d?ng h? th?p phân	*/		
char nibbleToHexCharacter(uint8_t nibble)
{
	return (nibble < 10 ? '0' + nibble : 'A' + nibble - 10);
}

void printHexByte(uint8_t byte)
{
	uint8_t upperNibble = byte >> 4;
	uint8_t lowerNibble = byte & 0x0F;
	transmitByte(upperNibble >= 10 ? 'A' + (upperNibble - 10) : '0' + upperNibble);
	transmitByte(lowerNibble >= 10 ? 'A' + (lowerNibble - 10) : '0' + lowerNibble);
}
	
/*	Nh?n 3 ch? cái ASCII và chuy?n chúng thành m?t byte khi nh?n Enter ('\r')	*/
uint8_t getNumber(void)
{
	// Nh?n 1 s? 0 - 255 t? c?ng Serial và chuy?n chu?i ký t? thành s? t??ng ?ng
	char hundreds = '0';
	char tens = '0';
	char ones = '0';
	char thisChar = '0';
	do 
	{                    
		// D?ch các hàng sang bên trái <-
		hundreds = tens;
		tens = ones;
		ones = thisChar;
		thisChar = receiveByte();                   // Nh?n m?t ký t? m?i
		transmitByte(thisChar);                     // Truy?n l?i ký t? v?a nh?n ???c ?? xác nh?n
	} while (thisChar != '\r');                     // L?p ??n khi g?p ký t? return 
	return (100 * (hundreds - '0') + 10 * (tens - '0') + ones - '0');
}

