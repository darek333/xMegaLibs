/* 
* PortControl.cpp
*
* Created: 08.11.2020 10:17:14
* Author: tdare
*/


#include "PortControl.h"

// default constructor
PortControl::PortControl(volatile uint8_t * _PortPointer)
{
	portPointer = _PortPointer;
	directionRegisterPointer = _PortPointer-1;
	pin = _PortPointer-2;
	/*
	pin0.init(portPointer, 0);
	pin1.init(portPointer, 1);
	pin2.init(portPointer, 2);
	pin3.init(portPointer, 3);
	pin4.init(portPointer, 4);
	pin5.init(portPointer, 5);
	pin6.init(portPointer, 6);
	pin7.init(portPointer, 7);*/
} //PortControl

// default destructor
PortControl::~PortControl()
{
} //~PortControl

void PortControl::writePort(uint8_t value)
{
	*portPointer = value;
}
uint8_t PortControl::readPort()
{
	return *portPointer;
}

void PortControl::setPortAsInput()
{
	*directionRegisterPointer = 0xff;
}

void PortControl::stePortAsOutput()
{
	*directionRegisterPointer = 0;
}