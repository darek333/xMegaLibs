//-------------------------------------------------------------------------------------------------
// Wyświetlacz alfanumeryczny ze sterownikiem HD44780
// Sterowanie w trybie 4-bitowym bez odczytu flagi zajętości
// z dowolnym przypisaniem sygnałów sterujących
// Plik : HD44780.c
// Mikrokontroler : Atmel AVR
// Kompilator : avr-gcc
//-------------------------------------------------------------------------------------------------

#include "hd44780.h"
//-------------------------------------------------------------------------------------------------
//
// Funkcja wystawiająca półbajt na magistralę danych
//
//-------------------------------------------------------------------------------------------------
hd44780::hd44780(volatile uint8_t * RS_PortPointer, uint8_t RS_pinNumber, volatile uint8_t * E_PortPointer, uint8_t E_pinNumber, volatile uint8_t * DB4_PortPointer, uint8_t DB4_pinNumber, volatile uint8_t * DB5_PortPointer, uint8_t DB5_pinNumber,volatile uint8_t * DB6_PortPointer, uint8_t DB6_pinNumber,volatile uint8_t * DB7_PortPointer, uint8_t DB7_pinNumber)
{
	rsPin.initialization(RS_PortPointer, RS_pinNumber);
	ePin.initialization(E_PortPointer, E_pinNumber);
	DB4Pin.initialization(DB4_PortPointer, DB4_pinNumber);
	DB5Pin.initialization(DB5_PortPointer, DB5_pinNumber);
	DB6Pin.initialization(DB6_PortPointer, DB6_pinNumber);
	DB7Pin.initialization(DB7_PortPointer, DB7_pinNumber);
}

void hd44780::setHalfData(unsigned char haldOfData)
{
	if(haldOfData & 0x01)
	DB4Pin.setPin();//LCD_DB4_PORT |= LCD_DB4;
	else
	DB4Pin.clearPin();	//LCD_DB4_PORT  &= ~LCD_DB4;

	if(haldOfData & 0x02)
	DB5Pin.setPin(); //LCD_DB5_PORT |= LCD_DB5;
	else
	DB5Pin.clearPin(); //LCD_DB5_PORT  &= ~LCD_DB5;

	if(haldOfData & 0x04)
	DB6Pin.setPin(); //LCD_DB6_PORT |= LCD_DB6;
	else
	DB6Pin.clearPin(); //LCD_DB6_PORT  &= ~LCD_DB6;

	if(haldOfData & 0x08)
	DB7Pin.setPin(); //LCD_DB7_PORT |= LCD_DB7;
	else
	DB7Pin.clearPin(); //LCD_DB7_PORT  &= ~LCD_DB7;
}

//-------------------------------------------------------------------------------------------------
//
// Funkcja zapisu rozkazu do wyświetlacza
//
//-------------------------------------------------------------------------------------------------
void hd44780::WriteCommand(unsigned char commandToWrite)
{
	rsPin.clearPin();
	
	ePin.setPin();
	setHalfData(commandToWrite >> 4);
	ePin.clearPin();
	//It needs only 10ns between this commands so it can be one by another. 
	ePin.setPin();
	setHalfData(commandToWrite);
	ePin.clearPin();
	
	_delay_us(60);
}
//-------------------------------------------------------------------------------------------------
//
// Funkcja zapisu danych do pamięci wyświetlacza
//
//-------------------------------------------------------------------------------------------------
void hd44780::WriteData(unsigned char dataToWrite)
{
	//LCD_RS_PORT |= LCD_RS;
	rsPin.setPin();
	ePin.setPin();
	setHalfData(dataToWrite >> 4);
	ePin.clearPin();
	//It needs only 10ns between this commands, so it can can be one by another.
	ePin.setPin();
	setHalfData(dataToWrite);
	ePin.clearPin();
}
//-------------------------------------------------------------------------------------------------
//
// Funkcja wyświetlenia napisu na wyswietlaczu.
//
//-------------------------------------------------------------------------------------------------
void hd44780::LCD_WriteText(char * text)
{
	while(*text)
	{
		WriteData(*text++);
		_delay_us(60);
	}
	
}

void hd44780::LCD_Initalize(void)
{
	//unsigned char i;
	/*LCD_DB4_DIR |= LCD_DB4; // Konfiguracja kierunku pracy wyprowadzeń
	LCD_DB5_DIR |= LCD_DB5; //
	LCD_DB6_DIR |= LCD_DB6; //
	LCD_DB7_DIR |= LCD_DB7; //
	LCD_E_DIR 	|= LCD_E;   //
	LCD_RS_DIR 	|= LCD_RS;  // */
	DB4Pin.setAsOutput();
	DB5Pin.setAsOutput();
	DB6Pin.setAsOutput();
	DB7Pin.setAsOutput();
	ePin.setAsOutput();
	rsPin.setAsOutput();
	
	_delay_ms(1000); // oczekiwanie na ustalibizowanie się napiecia zasilajacego
	rsPin.clearPin(); //LCD_RS_PORT &= ~LCD_RS; // wyzerowanie linii RS
	ePin.clearPin(); //LCD_E_PORT &= ~LCD_E;  // wyzerowanie linii E

	ePin.setPin(); //LCD_E_PORT |= LCD_E; //  E = 1
	setHalfData(0x03); // tryb 8-bitowy
	ePin.clearPin(); //LCD_E_PORT &= ~LCD_E; // E = 0
	_delay_ms(5); // czekaj 5ms
	
	ePin.setPin(); //LCD_E_PORT |= LCD_E; //  E = 1
	ePin.clearPin(); //LCD_E_PORT &= ~LCD_E; // E = 0
	_delay_us(100); // czekaj 100us
	
	ePin.setPin(); //LCD_E_PORT |= LCD_E; //  E = 1
	ePin.clearPin(); //LCD_E_PORT &= ~LCD_E; // E = 0
	_delay_us(100); // czekaj 100us

	ePin.setPin(); //LCD_E_PORT |= LCD_E; // E = 1
	setHalfData(0x02); // tryb 4-bitowy
	ePin.clearPin(); //LCD_E_PORT &= ~LCD_E; // E = 0
	
	generalSettings(false, true, false);	// LCD_WriteCommand(HD44780_FUNCTION_SET | HD44780_FONT5x7 | HD44780_TWO_LINE | HD44780_4_BIT); // interfejs 4-bity, 2-linie, znak 5x7
	//displayCursorSettings(false, false, false); //LCD_WriteCommand(HD44780_DISPLAY_ONOFF | HD44780_DISPLAY_OFF); // wyłączenie wyswietlacza
	writingSettings(false, true); //LCD_WriteCommand(HD44780_ENTRY_MODE | HD44780_EM_SHIFT_CURSOR | HD44780_EM_INCREMENT);// inkrementaja adresu i przesuwanie kursora
	displayCursorSettings(true, false, false); //LCD_WriteCommand(HD44780_DISPLAY_ONOFF | HD44780_DISPLAY_ON | HD44780_CURSOR_OFF | HD44780_CURSOR_NOBLINK); // włącz LCD, bez kursora i mrugania
	clear(); //LCD_WriteCommand(HD44780_CLEAR); // czyszczenie zawartosći pamieci DDRAM
	_delay_ms(2);
	goHome();
	_delay_ms(2);
}

//-------------------------------------------------------------------------------------------------
//
// Koniec pliku HD44780.c
//
//-------------------------------------------------------------------------------------------------
void hd44780::clear()
{
	WriteCommand(0x01);
}

void hd44780::goHome()
{
	WriteCommand(0x02);
}

void hd44780::goToXY(uint8_t xCoordinate, uint8_t yCoordinate)
{
	WriteCommand(0x80 | (xCoordinate + (0x40 * yCoordinate)));
}


void hd44780::writingSettings(bool shiftDisplay, bool increment)
{
	uint8_t temp = 0x04;
	if (shiftDisplay)
	{
		temp|=1;
	}
	if (increment)
	{
		temp|=2;
	}
	WriteCommand(temp);
}

void hd44780::displayCursorSettings(bool displayOn, bool cursorOn, bool cursorBlink)
{
	uint8_t temp=0x08;
	if (cursorBlink)
	{
		temp|=1;
	}
	if (cursorOn)
	{
		temp|= 2;
	}
	if (displayOn)
	{
		temp |= 4;
	}
	WriteCommand(temp);
}

void hd44780::shifting(bool display, bool toRight)
{
	uint8_t temp=0x10;
	if (display)
	{
		temp|=8;
	}
	if (toRight)
	{
		temp|=4;
	}
	WriteCommand(temp);
}

void hd44780::generalSettings(bool font5x10, bool twoLines, bool _8bitCommunication)
{
	uint8_t temp = 0x20;
	if (font5x10)
	{
		temp|=4;
	}
	if (twoLines)
	{
		temp|=8;
	}
	if (_8bitCommunication)
	{
		temp|=16;
	}
	WriteCommand(temp);
}
