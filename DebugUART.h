/*
 * DebugUART.h
 *
 * Helper functions for ATMEGA4809 Curiosity Nano CDC Serial Port
 * Serial port is USART3 on PB0 (TxD) and PB1 (RxD)
 * Created: 4/2/2019 7:29:45 AM
 *  Author: PancakePuppy (John White)
 */ 
#include <avr/io.h>

#ifndef DEBUGUART_H_
#define DEBUGUART_H_

// Defines
#ifndef F_CPU
#error "F_CPU required to be defined"
#define F_CPU 3333333UL
#endif

#ifndef DEBUG_BAUD
#error "DEBUG_BAUD required to be defined"
#define DEBUG_BAUD 19200UL
#endif

#define DEBUG_BAUDREG (((F_CPU) * 4UL) / (DEBUG_BAUD))

// Function Prototypes
void DebugUART_Init();
void DebugUART_TxByte(uint8_t);
void DebugUART_TxString(char*);

#endif /* DEBUGUART_H_ */