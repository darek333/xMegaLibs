/* 
* PinControl.h
*
* Created: 08.11.2020 10:15:12
* Author: tdare
*/


#ifndef __PINCONTROL_H__
#define __PINCONTROL_H__
#include <stdint-gcc.h>


class PinControl
{
//variables
public:
protected:
private:
volatile uint8_t * portPointer;
volatile uint8_t * directionRegisterPointer;
volatile uint8_t * pin;
uint8_t pinNumber;

//functions
public:
	PinControl();
	~PinControl();
	
	void setPin();
	void clearPin();
	void setAsOutput();
	void setAsInput();
	uint8_t readPin();
	void initialization(volatile uint8_t * _PortPointer, uint8_t _pinNumber);
protected:
private:
	PinControl( const PinControl &c );
	PinControl& operator=( const PinControl &c );

}; //PinControl

#endif //__PINCONTROL_H__
