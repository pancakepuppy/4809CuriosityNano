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
void TWIM_Init();
uint8_t TWIM_Communicate(uint8_t addr, uint8_t * data, uint8_t length);
uint8_t TWIM_AddressPhase(uint8_t addr);
uint8_t TWIM_TransmitData(uint8_t data);
uint8_t TWIM_ReceiveData(uint8_t *data);
void TWIM_EndTransaction();

#endif /* TWI_H_ */