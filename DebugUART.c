/*
 * DebugUART.c
 *
 * Created: 4/2/2019 7:29:57 AM
 *  Author: PancakePuppy (John White)
 */ 
#define F_CPU 3333333UL // Set F_CPU
#define DEBUG_BAUD 19200UL // Set DEBUG_BAUD rate
#include "DebugUART.h"
#include <avr/io.h>

void DebugUART_Init()
{
	USART3_BAUD = DEBUG_BAUDREG;
	USART3_DBGCTRL = USART_DBGRUN_bm; // Enable run during debug
	PORTB_DIRSET = PIN0_bm; // Set TxD as output
	USART3_CTRLB = USART_TXEN_bm; // Enable transmitter
}

void DebugUART_TxByte(uint8_t tx_byte)
{
	while(!(USART3_STATUS & USART_DREIF_bm)); // Wait for Data Register Empty flag
	USART3_TXDATAL = tx_byte; // Load byte into Transmit Data Register
	while(!(USART3_STATUS & USART_TXCIF_bm)); // Wait for Transmit Complete flag
	USART3_STATUS |= USART_TXCIF_bm; // Clear Transmit Complete flag
}

void DebugUART_TxString(char *string)
{
	for(int i = 0; *(string + i) != '\0'; i++) // Loop through the string until hitting NULL character
	{
		DebugUART_TxByte((char)*(string + i)); // Print a byte
	}	
}
