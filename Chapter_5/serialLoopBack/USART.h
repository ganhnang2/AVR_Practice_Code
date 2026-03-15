/* 
	Hàm ?? kh?i t?o, truy?n (transmit), nh?n (receive) ký t? qua USART
	initUSART() yêu c?u BAUD ???c ??nh ngh?a ?? tính toán h? s? nhân t?c
	?? truy?n bit (bit-rate multiplier)
 */
 
#ifndef USART_RS232_H
#define USART_RS232_H

#ifndef BAUD                          /* N?u không ???c ??nh ngh?a trong Makefile... */
#define BAUD  9600                    /* ??t m?t baud rate default... */
#endif

// N?u file <util/delay.h> t?n t?i thì undefine F_CPU
#ifdef F_CPU
#undef F_CPU
#endif

// redefine clock CPU: 12MHz
#define F_CPU 12000000UL

/* ??nh ngh?a ?? thu?n l?i cho vi?c s? d?ng v? sau	*/
#define   USART_HAS_DATA   (UCSR0A & (1 << RXC0))
#define   USART_READY      (UCSR0A & (1 << UDRE0))

/* Nh?n F_CPU và BAUD ?ã ???c ??nh ngh?a, tính toán h? s?
	nhân bit-rate theo công th?c trong datasheet - cái này t?
	???c tính toán trong <hàm setbaud.h>			*/
void initUSART(void);

/* 	Kh?i hàm truy?n (transmit) và nh?n (receive) d? li?u t? thanh ghi UDR0
	Khi g?i hàm receiveByte() thì ch??ng trình c?a b?n s? b? treo
	??n khi có d? li?u ??n.		*/
void transmitByte(uint8_t data);
uint8_t receiveByte(void);

/* 	Hàm ti?n ích ?? truy?n toàn b? chu?i ký t? t? RAM	*/
void printString(const char myString[]);

/* 	??nh ngh?a m?t bi?n char[] truy?n vào hàm, bi?n này s? l?u tr?
	t?t c? nh?ng gì b?n ?ã truy?n qua c?ng Serial	*/
void readString(char myString[], uint8_t maxLength);

/*	In byte thành m?t s? ASCII 3-digits t??ng ?ng 
	Ví d?: byte = 56 -> T??ng ???ng "0", "5", "6" trong mã ASCII và truy?n
	b?ng USART		*/
void printByte(uint8_t byte);

/*	In ra m?t word (16-bits) thành m?t s? ASCII 5-digit t??ng ?ng 	*/
void printWord(uint16_t word);

/* 	In ra m?t byte d??i d?ng h? nh? phân 0s và 1s	*/
void printBinaryByte(uint8_t byte);
                                     
/* 	In ra m?t byte d??i d?ng h? th?p phân	*/								 
char nibbleToHexCharacter(uint8_t nibble);
void printHexByte(uint8_t byte);
                               
/*	Nh?n 3 ch? cái ASCII và chuy?n chúng thành m?t byte khi nh?n Enter ('\r')	*/
uint8_t getNumber(void);


#endif