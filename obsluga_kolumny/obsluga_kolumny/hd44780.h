//-------------------------------------------------------------------------------------------------
// Wyúwietlacz alfanumeryczny ze sterownikiem HD44780
// Sterowanie w trybie 4-bitowym bez odczytu flagi zajÍtoúci
// z dowolnym przypisaniem sygna≥Ûw sterujπcych
// Plik : HD44780.h
// Mikrokontroler : Atmel AVR
// Kompilator : avr-gcc
// Autor : Rados≥aw KwiecieÒ
// èrÛd≥o : http://radzio.dxp.pl/hd44780/
// Data : 24.03.2007
//-------------------------------------------------------------------------------------------------

#include <avr/io.h>
#include <util/delay.h>
#include "PinControl.h"

class hd44780
{
	private:
	
	PinControl rsPin;
	PinControl ePin;
	PinControl DB4Pin;
	PinControl DB5Pin;
	PinControl DB6Pin;
	PinControl DB7Pin;
	
	void setHalfData(unsigned char haflOfData);
	void WriteCommand(unsigned char);
	
	public:
	
	void WriteData(unsigned char);// Display one character
	void LCD_WriteText(char *);// Display table of characters
	void LCD_Initalize(void);// Initialization of LCD
	void displayCursorSettings(bool displayOn, bool cursorOn, bool cursorBlink);// Setting id display must be on; if cursor should be on; if it should blinking
	void shifting(bool display, bool toRight);//If display has shift or cursor; if it should shift to right or left 
	void generalSettings(bool font5x10, bool twoLines, bool _8bitCommunication);//If font 5x10 or 5x7; If 2 lines or 1; If 8 or 4 lines communication;
	void writingSettings(bool shiftDisplay, bool increment);//If display should be shifted or cursor; If it should increment or decrement.
	void clear();
	void goHome();
	void goToXY(uint8_t xCoordinate, uint8_t yCoordinate);
	hd44780(volatile uint8_t * RS_PortPointer, uint8_t RS_pinNumber, volatile uint8_t * E_PortPointer, uint8_t E_pinNumber, volatile uint8_t * DB4_PortPointer, uint8_t DB4_pinNumber, volatile uint8_t * DB5_PortPointer, uint8_t DB5_pinNumber,volatile uint8_t * DB6_PortPointer, uint8_t DB6_pinNumber,volatile uint8_t * DB7_PortPointer, uint8_t DB7_pinNumber);
};


//-------------------------------------------------------------------------------------------------
//
// Instrukcje kontrolera Hitachi HD44780
//
//-------------------------------------------------------------------------------------------------
/*
#define HD44780_CLEAR					0x01

#define HD44780_HOME					0x02

//writingSettings()
#define HD44780_ENTRY_MODE				0x04
#define HD44780_EM_SHIFT_CURSOR		0
#define HD44780_EM_SHIFT_DISPLAY	1
#define HD44780_EM_DECREMENT		0
#define HD44780_EM_INCREMENT		2

// displayCursorSetting()
#define HD44780_DISPLAY_ONOFF			0x08
#define HD44780_DISPLAY_OFF			0
#define HD44780_DISPLAY_ON			4
#define HD44780_CURSOR_OFF			0
#define HD44780_CURSOR_ON			2
#define HD44780_CURSOR_NOBLINK		0
#define HD44780_CURSOR_BLINK		1

//Shifting()
#define HD44780_DISPLAY_CURSOR_SHIFT	0x10
#define HD44780_SHIFT_CURSOR		0
#define HD44780_SHIFT_DISPLAY		8
#define HD44780_SHIFT_LEFT			0
#define HD44780_SHIFT_RIGHT			4

//generalSetting()
#define HD44780_FUNCTION_SET			0x20
#define HD44780_FONT5x7				0
#define HD44780_FONT5x10			4
#define HD44780_ONE_LINE			0
#define HD44780_TWO_LINE			8
#define HD44780_4_BIT				0
#define HD44780_8_BIT				16

#define HD44780_CGRAM_SET				0x40

#define HD44780_DDRAM_SET				0x80
*/
//-------------------------------------------------------------------------------------------------
//
// Koniec pliku HD44780.h
//
//-------------------------------------------------------------------------------------------------
