/*
 * TWI.c
 *
 * Created: 4/2/2019 1:29:45 PM
 *  Author: PancakePuppy (John White)
 */ 
#include <avr/io.h>
#include "TWI.h"

// TWIM_Init initializes the TWI Master peripheral
void TWIM_Init()
{
	TWI0_DBGCTRL = TWI_DBGRUN_bm; // Enable run during debug
	TWI0_MCTRLA = TWI_SMEN_bm; // Enable Smart Mode (Auto-ACK after reading MDATA register)
	TWI0_MBAUD = 10; // 94.44kHz with 3.333MHz F_CPU and 2us Trise
	TWI0_MCTRLA |= TWI_ENABLE_bm; // Enable TWI0
	TWI0_MSTATUS = TWI_BUSSTATE_IDLE_gc; // Set bus state to IDLE
	return;
}

// TWIM_Communicate performs TWI Master reads and writes.
// Arguments:
//			addr: uint8_t, bit 0 determines read (1) or write (0)
//			data: pointer to uint8_t, points to data transmit or receive buffer
//			length: uint8_t, function will attempt to send or receive this many
//					bytes.
// Return:
//			uint8_t, number of data bytes transmitted or received
//					Returns 0 if address phase is not ACKed
uint8_t TWIM_Communicate(uint8_t addr, uint8_t *data, uint8_t length)
{
	uint8_t is_read = addr & TWI_READ;	// Determine if the transaction is READ or WRITE by address LSB
	uint8_t byte_count = 0;				// Data bytes counter
	TWI0_MADDR = addr;					// Write slave address into MADDR to begin a transaction
	while(!(TWI0_MSTATUS & (TWI_RIF_bm|TWI_WIF_bm))); // Wait for address phase to complete
	
	if(TWI0_MSTATUS & (TWI_RXACK_bm|TWI_ARBLOST_bm)) // If address NACKed or arbitration lost...
	{
		TWI0_MCTRLB = TWI_MCMD_STOP_gc; // Send STOP command to end transaction
		return byte_count;
	}
	
	if(is_read) // Handle data reception for read transaction
	{
		while(byte_count < length)
		{
			if(TWI0_MSTATUS & ( TWI_ARBLOST_bm|TWI_BUSERR_bm|TWI_RXACK_bm))
				break; // Break out of the loop if bus error/arbitration lost/slave NACKed
			*(data + byte_count) = TWI0_MDATA;
			byte_count++;
		}
		TWI0_MCTRLB = TWI_MCMD_STOP_gc; // Send STOP command to end transaction
		return byte_count;
	}
	
	else // Handle data transmission for write transaction
	{
		while(byte_count < length)
		{
			TWI0_MDATA = *(data + byte_count); // Start transmit operation by writing master data register
			while(!(TWI0_MSTATUS & TWI_WIF_bm)); // Wait for Write Interrupt Flag set
			if(TWI0_MSTATUS & ( TWI_ARBLOST_bm|TWI_BUSERR_bm|TWI_RXACK_bm))
				break; // Break out of the loop if bus error/arbitration lost/slave NACKed
			byte_count++;
		}
		TWI0_MCTRLB = TWI_MCMD_STOP_gc; // Send STOP command to end transaction
		return byte_count;
	}
	
}