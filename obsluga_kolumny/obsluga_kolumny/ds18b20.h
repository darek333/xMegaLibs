/* 
* ds18b20.h
*
* Created: 11.11.2020 14:18:52
* Author: tdare
*/


#ifndef __DS18B20_H__
#define __DS18B20_H__
#include <stdlib.h>
#include <stdint-gcc.h>
#include "PinControl.h"
#include <util/delay.h>


class ds18b20
{
//variables

public:

/*
const uint8_t readRom = 0x33; //Wywo³anie o podanie swojego numeru. Dzia³a tylko dla jednego uk³adu podpiêtego pod dany interface.
const uint8_t matchRom = 0x55; //Komenda informuj¹ca, ¿e za chwile zostanie wywo³any konkretny odbiorca.
const uint8_t SkipRom = 0xCC; //Komenda oznajmiaj¹ca adresowanie do wszystkich.
const uint8_t SearchRom = 0xF0; //
const uint8_t AlarmSearch = 0xEC;

const uint8_t writeScratchpad = 0x4E;
const uint8_t readScratchpad = 0xBE;
const uint8_t Scratchpad = 0x48;
const uint8_t convertTemp = 0x44; //Demend temp convert 
const uint8_t recallE2 = 0xB8;
const uint8_t readPowerSupply = 0xB4;*/

protected:

private:
PinControl dataPin;
//functions
public:
	ds18b20(volatile uint8_t *_dataPort, uint8_t _dataPinNumber);
	~ds18b20();
	uint8_t initialization();
	uint8_t reset();
	void writeSingleBit(bool bit);
	uint8_t readSingleBit();
	void searchAddreses(long long *addres, int numberOfDevices);
	uint8_t calculateCRC (long long data, uint8_t length);
	long long readSingleAddres();
	void writeCommand(long long *data, uint8_t numberOfBits);
	long long readData(uint8_t numberOfBits);
	void writeByte(uint8_t data);
	uint8_t readByte();
protected:
private:
	ds18b20( const ds18b20 &c );
	ds18b20& operator=( const ds18b20 &c );

}; //ds18b20

#endif //__DS18B20_H__
