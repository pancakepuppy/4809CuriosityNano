/*
 * TWI.c
 *
 * Created: 4/2/2019 1:29:45 PM
 *  Author: PancakePuppy (John White)
 */ 
#include <avr/io.h>

// Initialize the TWI0 peripheral in Master mode
void TWIM_init()
{
	TWI0_DBGCTRL = TWI_DBGRUN_bm; // Enable run during debug
	TWI0_MCTRLA = TWI_SMEN_bm; // Enable Smart Mode (Auto-ACK after reading MDATA register)
	TWI0_MBAUD = 12; // Appropriate for ~100kHz at 3.333MHz PER_CLK
	TWI0_MCTRLA |= TWI_ENABLE_bm; // Enable TWI0
	TWI0_MSTATUS = TWI_BUSSTATE_IDLE_gc; // Set bus state to IDLE
}

uint8_t TWIM_communicate(uint8_t addr, uint8_t *data, uint8_t length)
{
	uint8_t is_read = addr & TWI_READ;
	uint8_t byte_count = 0; // Var to keep track of how much data we've sent/received
	TWI0_MADDR = addr; // Write slave address into MADDR to begin a transaction. Bit 0 = Read/Write#
	while(!(TWI0_MSTATUS & TWI_WIF_bm)); // Wait for address phase to complete
	if(TWI0_MSTATUS & (TWI_RXACK_bm|TWI_ARBLOST_bm)
		return byte_count; // Address not ACKed or arbitration lost, return from function
	if(is_read)
	{
		while(byte_count < length)
		{
			while(!(TWI0_MSTATUS & TWI_RIF_bm)); // Wait for Receive Interrupt Flag set
			*(data + byte_count) = TWI0_MDATA;
			byte_count++;
		}
		TWI0_MCTRLB = TWI_MCMD_STOP_gc; // Send STOP command to end transaction
	}
	else // Is a write operation
	{
		while(byte_count < length)
		{
			TWI0_MDATA = *(data + byte_count); // Start transmit operation by writing master data register
			while(!(TWI0_MSTATUS & TWI_WIF_bm)); // Wait for Write Interrupt Flag set
			byte_count++;
		}
		TWI0_MCTRLB = TWI_MCMD_STOP_gc; // Send STOP command to end transaction
	}
	
}