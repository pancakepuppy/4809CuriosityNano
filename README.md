# 4809CuriosityNano
Convenience libraries for the ATMEGA4809 Curiosity Nano

This repository contains convenience libraries for the ATMEGA4809. This is intended to be a mix of programming practice for me and 
a way to get up and running quickly with this new chip's peripherals. The code is in a constant state of flux as I figure out what
functions I need to achieve whatever I want to do.

These depend upon a version of AVR libc new enough to have an io.h for the ATMEGA4809. I've tested functionality against an ATMEGA4809
Curiosity Nano development board from Microchip running with the default 3.333MHz F_CPU (20M oscillator, DIV6 prescaler) and Atmel 
Studio 7.

TWI.c/.h
Two Wire Interface (Master)
---------------------------
The TWI files contain functions for using the TWI interface in I2C Master mode at ~100kHz.
    void TWIM_Init() - Initializes the TWI Master peripheral and changes bus state to IDLE.
    uint8_t TWIM_AddressPhase(uint8_t addr) - Attempts to take ownership of the bus and start a transaction by writing the slave address.
                                              The LSB of addr is R/W#. 7-bit addresses should be left shifted once, then ORed with R/W#.
                                              Function returns 1 and ends transaction if slave doesn't ACK, arbitration is lost, or there 
                                              is a bus error; 0 if successful.
    void TWIM_EndTransaction() - Sends the TWI peripheral a STOP command to end the transaction and return bus to IDLE.
    uint8_t TWIM_ReceiveData(uint8_t *data) - Loads most recently received byte into *data and starts another read if TWI0_MCTRLB.ACKACT==0.
                                              Function returns 1 if arbitration is lost or there is a bus error; 0 if successful.
    uint8_t TWIM_TransmitData(uint8_t data) - Transmits data byte. Returns 1 and ends transaction if slave doesn't ACK, arbitration is lost,
                                              or there is a bus error; 0 if successful.
    uint8_t TWIM_Communicate(uint8_t addr, uint8_t *data, uint8_t length) - 
                                              Performs an I2C transaction with slave at addr with *data of length bytes. The function returns
                                              the number of bytes moved during the transaction, or 0 if the address phase fails.

Piecewise communication: TWIM_Init() -> TWIM_AddressPhase() -> TWIM_ReceiveData()/TWIM_TransmitData() (ad nauseum) -> TWIM_EndTransaction()
Bundled communication: TWIM_Init() -> TWIM_Communicate()
