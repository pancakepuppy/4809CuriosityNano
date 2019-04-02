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
	TWI0_MBAUD = 12;
	TWI0_MCTRLA |= TWI_ENABLE_bm; // Enable TWI0
	TWI0_MSTATUS = TWI_BUSSTATE_IDLE_gc; // Set bus state to IDLE
}

void TWIM_communicate(uint8_t addr, uint8_t *data, uint8_t length)
{
	TWI0_MADDR = addr; // Write slave address into MADDR to begin a transaction. Bit 0 = Read/Write#
	while(!(TWI0_MSTATUS & TWI_WIF_bm)); // Wait for address phase to complete
	if(TWI0_MSTATUS & TWI_RXACK_bm)
		return; // Address not ACKed, return from function
	
}