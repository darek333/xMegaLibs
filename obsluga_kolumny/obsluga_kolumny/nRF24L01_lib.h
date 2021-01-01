/*
 * nRF24L01_lib.h
 *
 * Created: 01.01.2021 01:07:16
 *  Author: tdare
 */ 
#include "communication.h"

#ifndef NRF24L01_LIB_H_
#define NRF24L01_LIB_H_

class nRF24L01
{
	private:
	SPI_class *SPI_handler;
	PinControl CSNpinHandler;
	PinControl CEpinHandler;
	public:
	
	private:
	uint8_t writeWithNRF24L01(uint8_t command, uint8_t * dataToSend, uint8_t * dataReceived, uint8_t dataLongitude);
	public:
	nRF24L01(SPI_class *_SPI_handler, volatile uint8_t * SSport, uint8_t SSpinNumber);
	uint8_t readRegister(uint8_t * data, uint8_t dataLongitude, uint8_t addres);
	uint8_t writeRegister(uint8_t * data, uint8_t dataLongitude, uint8_t addres);
	uint8_t readRXPayload(uint8_t * data, uint8_t dataLongitude);
	uint8_t writeTXPayload(uint8_t * data, uint8_t dataLongitude);
	uint8_t flushTX();
	uint8_t flushRX();
	uint8_t reUseTxPl();
	uint8_t Nop();
};



#endif /* NRF24L01_LIB_H_ */