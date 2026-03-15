/*
	Serial Interrupt
	Lap trinh giao tiep noi tiep AVR su dung ngat
	+) Lap trinh bang phuong phap tham do (Polling) lam lang phi thoi gian cua bo
	nho -> Khi phai dung delay de thuc hien
	+) De kich hoat truyen du lieu dua tren ngat, ta can dat muc HIGH cho bit
	UDRIE0 (USART Data Register Interrupt Enable)
	+) De kich hoat nhan du lieu dua tren ngat, ta can dat muc HIGH cho bit
	RXCIE0 (Receive Complete Interrupt Enable) trong thanh ghi USART0B
	
	-> Trong bai code nay, kiem tra chuc nang ngat doi voi Receive (MCU nhan data)
 */ 

#define F_CPU 12000000UL								// Clock CPU: 12MHz
#include <avr/io.h>
#include <stdio.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#define BAUD_RATE	9600								// Baud: 9600bps
#define UBRR_VAL ((F_CPU/(BAUD_RATE*8UL)) - 1)			// Register USART Baud Rate Register: UBBR, mode: Asynchronous double speed mode

// USART initialization
void USART_init(void)
{
	// UCSR0A |= (1 << U2X0);
	// Double speed mode
	UCSR0A = 0b00000010;
	
	// UCSR0B = 0b00011000
	// bit 4:3 = 1 -> Transmit, Receive mode enable
	// bit 7: 1 -> Active Interrupt for RXC0  (UCSR0A)
	// UCSR0B = (1 << RXEN0) | (1 << TXEN0) | (1 << RXCIE0);
	UCSR0B = 0b10011000;
	
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

// Interrupt for receiving
ISR(USART_RX_vect)
{
	// Reset PORTB, D
	PORTB = 0x00;
	PORTD = 0x00;
	
	// Control LED with UDR0
	// Bit 0 -> 5 = PORTD0:5
	// Bit 6 -> 7 = PORTD6:7
	PORTB = (UDR0 & ~(1 << PORTB6 | 1 << PORTB7));
	PORTD = (UDR0 & (1 << PORTD6 | 1 << PORTD7));
}

int main(void)
{
    USART_init();		// Initialize USART
	sei();				// Enable global interrupt
	DDRB = 0x3F;		// Configure pin B0:5 output
	DDRD = 0xC0;		// Configure pin D6:7 output
    while (1);
}

