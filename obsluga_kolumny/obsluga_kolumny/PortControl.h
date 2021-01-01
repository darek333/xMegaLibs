/* 
* PortControl.h
*
* Created: 08.11.2020 10:17:14
* Author: tdare
*/


#ifndef __PORTCONTROL_H__
#define __PORTCONTROL_H__
#include <stdint-gcc.h>
#include "PinControl.h"


class PortControl
{
//variables
public:
/*
PinControl pin0;
PinControl pin1;
PinControl pin2;
PinControl pin3;
PinControl pin4;
PinControl pin5;
PinControl pin6;
PinControl pin7;*/

void writePort(uint8_t value);
uint8_t readPort(); 
void setPortAsInput();
void stePortAsOutput();

protected:
private:
volatile uint8_t * portPointer;
volatile uint8_t * directionRegisterPointer;
volatile uint8_t * pin;
//functions
public:
	PortControl(volatile uint8_t * _PortPointer);
	~PortControl();
protected:
private:
	PortControl( const PortControl &c );
	PortControl& operator=( const PortControl &c );

}; //PortControl

#endif //__PORTCONTROL_H__
