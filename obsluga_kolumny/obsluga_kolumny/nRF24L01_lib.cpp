/*
 * nRF24L01_lib.cpp
 *
 * Created: 01.01.2021 01:08:44
 *  Author: tdare
 */ 
#include "nRF24L01"


nRF24L01::nRF24L01(SPI_class *_SPI_handler, volatile uint8_t * SSport, uint8_t SSpinNumber)
{
	
}

//Bazowa funkcja do komunikacji. Nie wiem czy nie bêdzie potrzebne opóŸnienie pomiedzy wgraniem do rejestru instrukcji a odczytem uzyskanych informacji. Na logikê powinno tam wyst¹piæ kilka do kilkudziesiêciu us. 
uint8_t nRF24L01::writeWithNRF24L01(uint8_t command, uint8_t * dataToSend = NULL, uint8_t * dataReceived = NULL, uint8_t dataLongitude = 0)
{
	uint8_t StatusRegister = 0;
	CSNpinHandler.clearPin();
	SPI_handler->SPIenable();
	SPI_handler->writeChar(command);
	StatusRegister = SPI_handler->readChar;
	for (int i=0; i< dataLongitude; i++)
	{
		if (dataToSend!=NULL)
		{
			SPI_handler->writeChar(dataToSend[i]);
		} 
		else
		{
			SPI_handler->writeChar(0x00);
		}
		if (dataReceived!=NULL)
		{
			dataReceived[i] = SPI_handler->readChar();
		} 
	}
	CSNpinHandler.setPin();
	return StatusRegister;
}

uint8_t nRF24L01::readRegister(uint8_t * data, uint8_t dataLongitude, uint8_t addres)
{
	uint8_t StatusRegister = 0;
	if (addres>0x17)
	{
		return 0;
	}
	StatusRegister = writeWithNRF24L01(addres, NULL, data, dataLongitude);
	return StatusRegister;
}

uint8_t nRF24L01::writeRegister(uint8_t * data, uint8_t dataLongitude, uint8_t addres)
{
	uint8_t StatusRegister = 0;
	if (addres>0x17)
	{
		return 0;
	}
	StatusRegister = writeWithNRF24L01(addres|0x20, data, NULL, dataLongitude);
	return StatusRegister;
}

uint8_t nRF24L01::readRXPayload(uint8_t * data, uint8_t dataLongitude)
{
	uint8_t StatusRegister = 0;
	CEpinHandler.clearPin();
	StatusRegister = writeWithNRF24L01(0x61, NULL, data, dataLongitude);
	CEpinHandler.setPin();
	return StatusRegister;
}

uint8_t nRF24L01::writeTXPayload(uint8_t * data, uint8_t dataLongitude)
{
	uint8_t StatusRegister = 0;
	CEpinHandler.clearPin();
	StatusRegister = writeWithNRF24L01(0xA0, NULL, data, dataLongitude);
	CEpinHandler.setPin();
	_delay_us(10);
	return StatusRegister;
}

uint8_t nRF24L01::flushTX()
{
	
}
uint8_t nRF24L01::flushRX();
uint8_t nRF24L01::reUseTxPl();
uint8_t nRF24L01::Nop();