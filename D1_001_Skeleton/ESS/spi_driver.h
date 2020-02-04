/**
 *	SPI Driver
 *
 *	@author 	Andrew Markham
 *	@version 	v1.0
 *	@ide		Keil uVision
 *	@license	GNU GPL v3
 */
#ifndef SPI_DRIVER_H
#define SPI_DRIVER_H

#include <stdint.h>

// Initialize the SPI 
void SPIAcc_Init(void);



// Send a byte on SPI 
// @param address address of register to write to 
// @param data byte to send 
void SPIAcc_SendByte(uint8_t address, uint8_t data);

// Receive a byte on SPI 
// @param address address of register to read from 
// @return byte of data 
uint8_t SPIAcc_GetByte(uint8_t address);

#endif