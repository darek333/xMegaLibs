/*
Biblioteka przeznaczona do uproszczenia komunikacji poprzez:
-SPI
*/

#ifndef __COMMUNICATION_H__
#define __COMMUNICATION_H__

#include <avr/io.h>
#include <stdint-gcc.h>
#include "PinControl.h"

class SPI_class
{
	private:
	volatile uint8_t * Spc_register;
	volatile uint8_t * Sps_register;
	volatile uint8_t * Spd_register;
	PinControl MOSIPinHandler;
	PinControl MISOPinHandler;
	PinControl SSPinHandler;
	PinControl SCKPinHandler;
	bool master;
	public:
	
	
	private:
	public:
	SPI_class(volatile uint8_t * _MOSIPortPointer, uint8_t _MOSIpinNumber, volatile uint8_t * _MISOPortPointer, uint8_t _MISOpinNumber, volatile uint8_t * _SSPortPointer, uint8_t _SSpinNumber, volatile uint8_t * _SCKPortPointer, uint8_t _SCKpinNumber, bool _master, uint8_t SCK_Devider, volatile uint8_t * _Spc_register, bool SPIInterruptEnable, bool SCKsecondEdgeActive, bool SCKnormallyHigh, bool MSBfirstTransmit);
	void SPIenable();
	void SPIdisable();
	void writeChar(char symbol);
	char readChar();
	void setAsMaster();
	void setAsSlave();
};




#endif


