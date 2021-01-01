
#include "communication.h"

//Constructor 
SPI_class::SPI_class(volatile uint8_t * _MOSIPortPointer, uint8_t _MOSIpinNumber, volatile uint8_t * _MISOPortPointer, uint8_t _MISOpinNumber, volatile uint8_t * _SSPortPointer, uint8_t _SSpinNumber, volatile uint8_t * _SCKPortPointer, uint8_t _SCKpinNumber, bool _master, uint8_t SCK_Devider, volatile uint8_t * _Spc_register, bool SPIInterruptEnable = false, bool SCKsecondEdgeActive = false, bool SCKnormallyHigh = false, bool MSBfirstTransmit = false)
{
	Spc_register = _Spc_register;
	Sps_register = _Spc_register+1;
	Spd_register = _Spc_register+2;
	master = _master;
	
	MOSIPinHandler.initialization(_MOSIPortPointer, _MOSIpinNumber);
	MISOPinHandler.initialization(_MISOPortPointer, _MISOpinNumber);
	SCKPinHandler.initialization(_SCKPortPointer, _SCKpinNumber);
	SSPinHandler.initialization(_SSPortPointer, _SSpinNumber);
	
	//Clock signal settings
	if (SCKsecondEdgeActive)
	{
		*Spc_register |= (1<<3);
	} 
	else
	{
		*Spc_register &= (~(1<<3));
	}
	if (SCKnormallyHigh)
	{
		*Spc_register |= (1<<2);
	} 
	else
	{
		*Spc_register &= (~(1<<2));
	}
	//SCK Frequency
	switch (SCK_Devider)
	{
		case 2:
		*Sps_register|=1;
		*Spc_register&=(~3);
		break;
		
		case 4:
		*Sps_register&=(~1);
		*Spc_register&=(~3);
		break;
		
		case 8:
		*Sps_register|=1;
		*Spc_register|=1;
		*Spc_register&=(~2);
		break;
		
		case 16:
		*Sps_register&=(~1);
		*Spc_register|=1;
		*Spc_register&=(~2);
		break;
		
		case 32:
		*Sps_register&=(~1);
		*Spc_register|=2;
		*Spc_register&=(~1);
		break;
		
		case 64:
		*Sps_register|=1;
		*Spc_register|=3;
		break;
		
		case 128:
		*Sps_register&=(~1);
		*Spc_register|=3;
		break;
	}
	
	// Master/Slave 
	if (master)
	{
		*Spc_register |= (1<<4);
		MOSIPinHandler.setAsOutput();
		SCKPinHandler.setAsOutput();
		MISOPinHandler.setAsInput();
		SSPinHandler.setAsOutput();
		SSPinHandler.setPin();
	} 
	else
	{
		*Spc_register &= (~(1<<4));
		MOSIPinHandler.setAsInput();
		SCKPinHandler.setAsInput();
		MISOPinHandler.setAsOutput();
		SSPinHandler.setAsInput();
		SSPinHandler.clearPin();
	}
	
	//Data order
	if (MSBfirstTransmit)
	{
		*Spc_register |= (1<<5);
	} 
	else
	{
		*Spc_register &= ~(1<<5);
	}
	
	//SPI interrupt enable
	if (SPIInterruptEnable)
	{
		*Spc_register |= (1<<7);
	} 
	else
	{
		*Spc_register &= ~(1<<7);
	}
}

//Enable SPI module 
void SPI_class::SPIenable()
{
	if (master)
	{
		MOSIPinHandler.setAsOutput();
		SCKPinHandler.setAsOutput();
		MISOPinHandler.setAsInput();
		SSPinHandler.setAsOutput();
		SSPinHandler.setPin();
	} 
	else
	{
		MOSIPinHandler.setAsInput();
		SCKPinHandler.setAsInput();
		MISOPinHandler.setAsOutput();
		SSPinHandler.setAsInput();
		SSPinHandler.clearPin();
	}
	*Spc_register |= (1<<6);
}

//Disable SPI module
void SPI_class::SPIdisable()
{
	*Spc_register &= ~(1<<6);
}

//Send single 1 byte 
void SPI_class::writeChar(char symbol)
{
	*Spd_register = symbol;
}

//Receive single char/one byte 
char SPI_class::readChar()
{
	return *Spd_register;
}

SPI_class::setAsMaster()
{
	*Spc_register |= (1<<4);
	MOSIPinHandler.setAsOutput();
	SCKPinHandler.setAsOutput();
	MISOPinHandler.setAsInput();
	SSPinHandler.setAsOutput();
	SSPinHandler.setPin();
}

SPI_class::SetAsSlave()
{
	*Spc_register &= (~(1<<4));
	MOSIPinHandler.setAsInput();
	SCKPinHandler.setAsInput();
	MISOPinHandler.setAsOutput();
	SSPinHandler.setAsInput();
	SSPinHandler.clearPin();
}
