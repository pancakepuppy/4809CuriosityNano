/*
 * TWI.c
 *
 * Created: 4/2/2019 1:29:45 PM
 *  Author: PancakePuppy (John White)
 */ 
#include <avr/io.h>
#include "TWI.h"

// Initialize the TWI0 peripheral in Master mode
void TWIM_Init()
{
	TWI0_DBGCTRL = TWI_DBGRUN_bm; // Enable run during debug
	TWI0_MCTRLA = TWI_SMEN_bm; // Enable Smart Mode (Auto-ACK after reading MDATA register)
	TWI0_MBAUD = 12; // Appropriate for ~100kHz at 3.333MHz PER_CLK
	TWI0_MCTRLA |= TWI_ENABLE_bm; // Enable TWI0
	TWI0_MSTATUS = TWI_BUSSTATE_IDLE_gc; // Set bus state to IDLE
}

uint8_t TWIM_Communicate(uint8_t addr, uint8_t *data, uint8_t length)
{
	uint8_t is_read = addr & TWI_READ;
	uint8_t byte_count = 0; // Var to keep track of how much data we've sent/received
	TWI0_MADDR = addr; // Write slave address into MADDR to begin a transaction. Bit 0 = Read/Write#
	while(!(TWI0_MSTATUS & (TWI_RIF_bm|TWI_WIF_bm))); // Wait for address phase to complete
	
	if(TWI0_MSTATUS & (TWI_RXACK_bm|TWI_ARBLOST_bm))
	{
		TWI0_MCTRLB = TWI_MCMD_STOP_gc; // Send STOP command to end transaction
		return byte_count; // Address not ACKed or arbitration lost, return from function
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