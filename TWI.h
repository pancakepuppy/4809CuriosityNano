/*
 * TWI.h
 *
 * Created: 4/2/2019 1:29:34 PM
 *  Author: PancakePuppy (John White)
 */ 


#ifndef TWI_H_
#define TWI_H_

#define TWI_READ 0x01
#define TWI_WRITE 0x00

// Function prototypes
void TWIM_init();
uint8_t TWIM_communicate(uint8_t, uint8_t*, uint8_t);

#endif /* TWI_H_ */