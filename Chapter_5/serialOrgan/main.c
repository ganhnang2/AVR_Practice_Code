/*	
	serialOrgan.c
	
	??c m?t ký t? t? keyboard qua USART, ch?i m?t note
 */ 

#include <avr/io.h>
#include "scale16.h"
#include "USART.h"
#include <util/delay.h>

/*		??nh ngh?a chân AVR c?m vào loa		*/
#define SPEAKER			PORTD5
#define SPEAKER_PORT	PORTD
#define SPEAKER_DDR		DDRD

/*		Xác ??nh ?? dài c?a note	*/
#define NOTE_DURATION	0xF000			

/*	Hàm ?? th?c hi?n ch?c n?ng phát ra các notes ??n loa (speaker)	*/
void playNote(uint16_t period, uint16_t duration) {
	uint16_t elapsed;
	uint16_t i;
	for (elapsed = 0; elapsed < duration; elapsed += period) {
		/* For loop with variable delay selects the pitch */
		for (i = 0; i < period; i++) {
			_delay_us(1);
		}
		SPEAKER_PORT ^= (1 << SPEAKER);		// ??o tr?ng thái bit (hay là t?n s? tín hi?u)
	}
}

void rest(uint16_t duration) {
	do {
		_delay_us(1);
	} while (--duration);
}

int main(void)
{
	// ----------------------Inits---------------------- //
	SPEAKER_DDR = (1 << SPEAKER);			/* ??t chân SPEAKER cho output */
	initUSART();
	printString("------------Serial Organ----------------\r\n");
	
	char fromCompy;							/* L?u tr? input serial */
	uint16_t currentNoteLength = NOTE_DURATION / 2;
	const uint8_t keys[] = { 'a', 'w', 's', 'e', 'd', 'f', 't',
		'g', 'y', 'h', 'j', 'i', 'k', 'o',
		'l', 'p', ';', '\''
	};
	const uint16_t notes[] = { G4, Gx4, A4, Ax4, B4, C5, Cx5,
		D5, Dx5, E5, F5, Fx5, G5, Gx5,
		A5, Ax5, B5, C6
	};
	uint8_t isNote;			// Ki?m tra xem có ?úng là note nh?c hay ko?
	uint8_t i;				
	
	//--------------Event Loop---------------------//
    while (1) 
    {
		printString("------------Serial Organ----------------\r\n");
		fromCompy = receiveByte();					// ??i ??n khi có input
		transmitByte('N');							// Báo cho máy tính r?ng chúng ta s?n sàng cho n?t ti?p theo
		
		/*	Play notes	*/
		isNote = 0;
		for (i = 0; i < sizeof(keys); i++)
		{
			if (fromCompy == keys[i])				// ?ã tìm th?y note trong b?ng notes[]
			{
				playNote(notes[i], currentNoteLength);
				isNote = 1;							// Ghi l?i vi?c chúng ta ?ã tìm th?y note
				break;
			}
		}
		
		/*	X? lý các phím không ph?i n?t nh?c: thay ??i t?c ?? (tempo) và ngh? (rests)	*/
		if (!isNote)
		{
			if (fromCompy == '[')						// Ký t? '[': short note
				currentNoteLength = NOTE_DURATION / 2;
			else if (fromCompy == ']')					// Ký t? ']': long note
				currentNoteLength = NOTE_DURATION;
			else
				rest(currentNoteLength);				// Ký t? ko xác ??nh, tr?ng thái ch?
		}
		
		/* End event loop */
    }
	
	return 0;
}

