/*
	I2C_Slave.c
	
	Programming steps in the master device
	1. Initialize I2C with slave device address.
	2. Listen to bus for get addressed by master.
	3. While addressed with SLA+W by master device, receive data from master device.
	4. Return acknowledgement after each byte received.
	5. Clear interrupt flag after REPEATED START/STOP received.
	6. Print received data on LCD.
	7. Again listen to bus for get addressed by master.
	8. While addressed with SLA+R by master device, transmit data to master device.
	9. Transmit data till NACK/REPEATED START/STOP receive from master.
	10. Clear interrupt flag after NACK/REPEATED START/STOP received.
 */ 

#define F_CPU		12000000UL		// Clock CPU: 12MHz
#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include "TWI_Slave.h"
#include "lcd16x02.h"

#define SLAVE_ADDRESS		0x02	// Own slave address: 0x02

int main(void)
{
    /*---------------Inits--------------*/
    int8_t data = 0;
	int8_t ackStatus = 0;					// 0: ACK -> Sending data, -2: NACK -> Stop transmitting data
    char buffer[10];
	uint8_t status;
    I2C_Slave_Init(SLAVE_ADDRESS);
    lcdBegin();
    printStringAtAnyPosition(0, 0, "Slave Device");
    
    /*------------Loop event------------*/
    while (1) 
    {
		data = 0;
		status = I2C_Slave_Listen();
		switch(status)
		{
			/*----------Slave Receiver Mode-----------*/
			case 0: 
				do 
				{
					sprintf(buffer, "Receive: %d", data);
					printStringAtAnyPosition(1, 13, buffer);
					data = I2C_Slave_Receive();		// receive data byte
				} 
				while (data > 0);				// loop until STOP/REAPEATED START condition is generated
					data = 0;
				break;
			/*----------End for Slave Receiver Mode-----------*/
			
			/*----------Slave Transmitter Mode-----------*/
			case 1:
				printStringAtAnyPosition(1, 0, "Sending: ");
				do
				{
					ackStatus = I2C_Slave_Transmit(data);	// transmit data byte
					sprintf(buffer, "%d", data);
					printStringAtAnyPosition(1, 13, buffer);
					data++;
				}
				while (ackStatus == 0);				// loop until STOP/REAPEATED START condition is generated
				break;
			/*----------End for Slave Transmitter Mode-----------*/
			default:
				break;
		}
    }
}

