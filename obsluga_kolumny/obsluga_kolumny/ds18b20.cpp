/* 
* ds18b20.cpp
*
* Created: 11.11.2020 14:18:52
* Author: tdare
*/


#include "ds18b20.h"

// default constructor
ds18b20::ds18b20(volatile uint8_t *_dataPort, uint8_t _dataPinNumber)
{
	dataPin.initialization(_dataPort, _dataPinNumber);
	dataPin.setAsInput();
	dataPin.clearPin();
} //ds18b20

// default destructor
ds18b20::~ds18b20()
{
} //~ds18b20

uint8_t ds18b20::initialization()
{
	dataPin.setAsOutput();
	dataPin.setPin();
	return 1;
}

uint8_t ds18b20::reset()
{
	uint8_t rV = 0;
	dataPin.setAsOutput();
	_delay_us(600); //zwiêkszone na potrzeby sprawdzenia
	dataPin.setAsInput();
	_delay_us(70);
	if (dataPin.readPin())
	{
		rV = 0;
	}
	else
	{
		rV= 1;
	}
	_delay_us(800); //zwiêkszone na potrzeby sprawdzenia
	return rV;
}

void ds18b20::writeSingleBit(bool bit)
{
	dataPin.clearPin();
	if (bit)
	{
		dataPin.setAsOutput();
		_delay_us(8);
		dataPin.setAsInput();
		_delay_us(80);
	} 
	else
	{
		dataPin.setAsOutput();
		_delay_us(80);		//Poprawka dla sprawdzenia
		dataPin.setAsInput();
		_delay_us(8);		//Poprawka dla sprawdzenia
	}
}

uint8_t ds18b20::readSingleBit()
{
	uint8_t data;
	dataPin.clearPin();
	dataPin.setAsOutput();
	_delay_us(1);
	dataPin.setAsInput();
	_delay_us(5);				//Poprawka dla sprawdzenia
	data = dataPin.readPin();
	_delay_us(60);				//Poprawka dla sprawdzenia
	return data;
}

void ds18b20::searchAddreses(long long *addres, int numberOfDevices)
{
	uint8_t lastChoise=0; //number of addres bit where in previous iteration algorithm last time chose 0 instead of 1
	uint8_t actualChoise=0; //number of addres bit where algorithm last time chose 0 instead of 1
	uint8_t readFirst=0; 
	uint8_t readSecound=0;
	long long tempAddres = 0; //addres
	long long oldAddres = 0; //addres from previous iteration.  
	
	for (int i=0; i<numberOfDevices; i++)
	{
		reset();
		for (int j=0; j<=64; j++)
		{
			readFirst = readSingleBit();
			readSecound  = readSingleBit();
			if (readFirst!=readSecound)
			{
				if (readFirst)
				{
					tempAddres|= (1<<j);
					writeSingleBit(true);
				}
				else
				{
					writeSingleBit(false);
				}
			} 
			else
			{
				if (readFirst==0)
				{
					if (j+1==lastChoise)
					{
						tempAddres|= (1<<(j));
						writeSingleBit(true);
					} 
					else
					{
						if (j+1<lastChoise)
						{
							if (oldAddres&(1<<j))
							{
								writeSingleBit(true);
								tempAddres |= (1<<j);
							}
							else
							{
								actualChoise=j+1;
								writeSingleBit(false);
							}
						} 
						else
						{
							actualChoise=j+1;
							writeSingleBit(false);
						}
					}
				}
				else
				{
					tempAddres = NULL;
					break;
				}
			}
		}
		lastChoise=actualChoise;
		addres[i]=tempAddres;
		oldAddres=tempAddres;
	}
}

uint8_t ds18b20::calculateCRC (long long data, uint8_t length)
{
	uint8_t CRC=0;
	bool temp = false;
	for (int i=0; i<length; i++)
	{
		temp = ((data^CRC)&1);
		CRC>>=1;
		if (temp)
		{
			CRC^=0x8C;
		}
	}
	return CRC;
}

long long ds18b20::readSingleAddres()
{
	///
}

void ds18b20::writeCommand(long long *data, uint8_t numberOfBits)
{
	for (int i=0; i< numberOfBits; i++)
	{
		writeSingleBit((*data)&(1<<i));
	}
}

void ds18b20::writeByte(uint8_t data)
{
	for (int i=0; i< 8; i++)
	{
		writeSingleBit(data&(1<<i));
	}
}

uint8_t ds18b20::readByte()
{
	uint8_t singleByte=0;
	for (int i=0; i< 8; i++)
	{
		singleByte|=(readSingleBit()<<i);
	}
	return singleByte;
}

long long ds18b20::readData(uint8_t numberOfBits)
{
	long long data=0;
	for (uint8_t i=0; i<numberOfBits; i++)
	{
		if (readSingleBit())
		{
			data|=(1<<i);
		}
	}
	return data;
}