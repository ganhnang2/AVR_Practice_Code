/*
	+) POV vi?t t?t cho persistence of vision (l?u ?nh c?a m?t), hi?n t??ng trong ?ó hình ?nh 
	sau khi nhìn v?n còn l?u l?i trên võng m?c kho?ng 1/25 giây
	-> Ý t??ng thi?t b? POV là b?n b?t t?t LED nhanh ??n m?c m?t ko k?p ph?n ?ng.
	-> B?n vung thi?t b? qua l?i, các m?u ánh sáng nó t?o ra s? l? l?ng trong ko gian
	
	+) ?? ch?i POV khai thác ph?n ?ng ch?m này ?? t?o ra nh?ng hình ?nh "l? l?ng" trong ko trung
	
	+) Vi?c gán MOSFET có vai trò nh? m?t công t?c ?? cách ly 2 chân GND l?n l??t c?a AVR và LED:
		- Khi mà ? ch? ?? n?p code -> Chân Reset = 0V -> MOSFET off 
		-> Cách ly gi?a chân GND (LED) và GND (AVTR) -> Khi n?p s? tránh b? nhi?u
		- Khi mà ? tr?ng thái bthg thì RESET = 5V -> MOSFET ON -> GND c?a c? 2 s? k?t n?i tr?c ti?p v?i nhau!
 */ 

#define  F_CPU 8000000UL
#include <avr/io.h>
#include <util/delay.h>

// Hàm th?c hi?n ?i?u khi?n ký t? in ra POV
void POVDisplay(uint8_t oneByte)
{
	PORTD = oneByte;		// Thi?t l?p c?u hình t?ng chân
	_delay_ms(2);
}

int main(void)
{
	// Kh?i t?o b?t t?t c? các chân nhóm D thành output
	DDRD = 0xFF;
	
    // L?p
    while (1) 
    {
		// Th?c hi?n thi?t l?p ?? in ra hình mong mu?n b?ng POV
		POVDisplay(0b00001110);
		POVDisplay(0b00011000);
		POVDisplay(0b10111101);
		POVDisplay(0b01110110);
		POVDisplay(0b00111100);
		POVDisplay(0b00111100);
		POVDisplay(0b00111100);
		POVDisplay(0b01110110);
		POVDisplay(0b10111101);
		POVDisplay(0b00011000);
		POVDisplay(0b00001110);
		
		// T?t c? các LED ?i
		PORTD = 0;
		_delay_ms(10);
    }
}

