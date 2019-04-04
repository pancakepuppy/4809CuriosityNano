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

// TWIM_Communicate performs bundled TWI Master reads and writes.
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
	if(TWIM_AddressPhase(addr))
	{
		return byte_count;
	}
	if(is_read) // Handle data reception for read transaction
	{
		TWI0_MCTRLB = TWI_ACKACT_ACK_gc; // Acknowledge Read Bytes
		while(byte_count < (length-1)) // Loop through until second to last byte
		{
			TWIM_ReceiveData(data + byte_count); // Each time MDATA is read, another read operation is started
			byte_count++;
		}
		TWI0_MCTRLB = TWI_ACKACT_NACK_gc; // Do not start another read operation
		TWIM_ReceiveData(data + byte_count);
		TWIM_EndTransaction();
		return byte_count;
	}
	else // Handle data transmission for write transaction
	{
		while(byte_count < length)
		{
			if(TWIM_TransmitData(data + byte_count))
				return byte_count;
			byte_count++;
		}
		TWIM_EndTransaction();
		return byte_count;
	}
	
}

// Piecewise transaction function - Address Phase
uint8_t TWIM_AddressPhase(uint8_t addr)
{
	TWI0_MADDR = addr;					// Write slave address into MADDR to begin a transaction
	while(!(TWI0_MSTATUS & (TWI_RIF_bm|TWI_WIF_bm))); // Wait for address phase to complete
	if(TWI0_MSTATUS & (TWI_RXACK_bm|TWI_ARBLOST_bm|TWI_BUSERR_bm)) // Check for NACK/BUSERR/ARBLOST
	{
		TWIM_EndTransaction();
		return 1; // Failed
	}
	return 0; // Success
}

// Piecewise transaction function - End Transaction
inline void TWIM_EndTransaction()
{
	TWI0_MCTRLB = TWI_MCMD_STOP_gc; // Send STOP command to end transaction
}

// Piecewise transaction function - Receive Data
uint8_t TWIM_ReceiveData(uint8_t *data)
{
	while(!(TWI0_MSTATUS & TWI_RIF_bm)); // Wait for read interrupt flag.
	*data = TWI0_MDATA; // Move received data into memory. Starts another read transaction when ACKACT is 0.
	if(TWI0_MSTATUS & ( TWI_ARBLOST_bm|TWI_BUSERR_bm)) // Check for BUSERR/ARBLOST
	{
		TWIM_EndTransaction(); // End transaction if there was an issue
		return 1; // Failed
	}
	return 0; // Success
}

// Piecewise transaction function - Transmit Data
uint8_t TWIM_TransmitData(uint8_t *data)
{
	TWI0_MDATA = *data; // Move byte into MDATA register to begin data transmission
	while(!(TWI0_MSTATUS & TWI_WIF_bm)); // Wait for Write Interrupt Flag set
	if(TWI0_MSTATUS & ( TWI_ARBLOST_bm|TWI_BUSERR_bm|TWI_RXACK_bm)) // Check for NACK/BUSERR/ARBLOST
	{
		TWIM_EndTransaction(); // End transaction if there was an issue
		return 1; // Failed
	}
	return 0; // Success
}