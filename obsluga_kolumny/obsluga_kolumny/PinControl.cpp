/* 
* PinControl.cpp
*
* Created: 08.11.2020 10:15:12
* Author: tdare
*/


#include "PinControl.h"

// default constructor
PinControl::PinControl()
{
}
/*PinControl::PinControl(volatile uint8_t * _PortPointer, uint8_t _pinNumber)
{
	portPointer = _PortPointer;
	directionRegisterPointer = _PortPointer-1;
	pin = _PortPointer-2;
	pinNumber = _pinNumber;
} //PinControl*/

// default destructor
PinControl::~PinControl()
{
} //~PinControl

void PinControl::initialization(volatile uint8_t * _PortPointer, uint8_t _pinNumber)
{
	portPointer = _PortPointer;
	directionRegisterPointer = _PortPointer-1;
	pin = _PortPointer-2;
	pinNumber = _pinNumber;
} //PinControl

void PinControl::setPin()
{
	*portPointer|=1<<pinNumber;
}

void PinControl::clearPin()
{
	*portPointer&=(~(1<<pinNumber));
}

void PinControl::setAsOutput()
{
	*directionRegisterPointer |=1<<pinNumber;
}

void PinControl::setAsInput()
{
	*directionRegisterPointer &=(~(1<<pinNumber));
}

uint8_t PinControl::readPin()
{
	if (*pin & (1<<pinNumber))
	{
		return 1;
	}
	else
	{
		return 0;
	}
}
