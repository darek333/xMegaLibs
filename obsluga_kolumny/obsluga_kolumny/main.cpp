/*
 * obsluga_kolumny.cpp
 *
 * Created: 13.06.2020 10:10:08
 * Author : tdare
 */ 

#include <avr/io.h>
#include <stdlib.h>
#include <util/delay.h>
#include "hd44780.h"
#include "avr/interrupt.h"
#include "timerCounterN.h"
#include "ds18b20.h"
#include "communication.h"

void writeHex(uint8_t znak, hd44780 *LCD);
/*
PinControl LED_pin;
PinControl buttonPin;

timerCounterN LED_timer(&TCCR0A, &TIMSK0);

volatile int counter=0;*/


int main(void)
{
	long long command = 0;
	long long addres = 0;
	char tab1[] = {'O','K'};
	char tab2[] = {'N', 'O', 'P', 'E'};
	char znak;
	char symbol = 'A';
	/*
	uint8_t data[9];
	uint8_t znak = 0;
	bool check = false;
	uint8_t CRC = 0;
	*/
	hd44780 LCD(&PORTA, 0, &PORTA, 2, &PORTA, 3, &PORTA, 4, &PORTA, 5, &PORTA, 6);
	LCD.LCD_Initalize();
	
	SPI_class SPI_handler(&PORTB, 5, &PORTB, 6, &PORTB, 4, &PORTB, 7, true, 64, &SPCR, false, false, false, false);
	/*
	PinControl triggerPin;
	triggerPin.initialization(&PORTC, 0);
	triggerPin.clearPin();
	triggerPin.setAsOutput();
	*/

	//PinControl pinRS;
	//pinRS.initialization(&PORTA, 2);
	sei();
	/*
	LED_pin.initialization(&PORTC, 7);
	buttonPin.initialization(&PORTC, 3);
	buttonPin.setAsInput();
	buttonPin.setPin();
	LED_pin.setAsOutput();
	LED_pin.setPin();
	LED_timer.delay_us(1000);
	*/
	
	//LCD.LCD_WriteText(tab1);
	
	ds18b20 termometr(&PORTC,5);
	DDRB |= 1<<5;
	/*
	CRC = termometr.calculateCRC(0x33, 8);
	check=(CRC==0x35);
	
	check = 0;*/
	/*
	termometr.reset();
	termometr.writeByte(0xCC);
	termometr.writeByte(0x4E);
	termometr.writeByte(0x00);
	termometr.writeByte(0x00);
	termometr.writeByte(0x00);
	_delay_us(1000);
	*/
	SPI_handler.SPIenable();
    while (1) 
    {
		/*
		if (!LED_timer.counting)
		{
			LED_timer.delay_us(0xf900);
		}*/
		
		/*
		termometr.reset();
		termometr.writeByte(0xCC);
		termometr.writeByte(0x44);
		_delay_ms(1000);
		termometr.reset();
		//triggerPin.setPin();
		termometr.writeByte(0xCC);
		termometr.writeByte(0xBE);
		//termometr.writeCommand(&command, 16);
		triggerPin.setPin();
		for (int i =0; i<9; i++)
		{
			data[i]=termometr.readByte(); 
		}
		triggerPin.clearPin();
		//termometr.initialization();
		*/
		
		SPI_handler.writeChar(symbol);
		
		_delay_ms(1);
		znak = SPI_handler.readChar();
		LCD.clear(); //LCD_WriteCommand(HD44780_CLEAR); // czyszczenie zawartosci pamieci DDRAM
		_delay_ms(2);
		LCD.goHome();
		_delay_ms(2);
		LCD.WriteData(znak);
		symbol+=1;
		_delay_ms(1500);
		/*
		for (int i =0; i<3; i++)
		{
			znak = ((data[2-i])>>4);
			writeHex(znak, &LCD);
			
			znak = (data[2-i])&0x0f;
			writeHex(znak, &LCD);
		}
		*/
	}
}

/*
ISR (TIMER0_COMPA_vect)
{
	counter++;
	if (counter==32)
	{
		if(LED_pin.readPin())
		{
			LED_pin.clearPin();
		}
		else
		{
			LED_pin.setPin();
		}
		counter=0;
	}
}*/

void writeHex(uint8_t znak, hd44780 *LCD)
{
	switch (znak)
	{
		case 0:
		LCD->WriteData('0');
		break;
		case 1:
		LCD->WriteData('1');
		break;
		case 2:
		LCD->WriteData('2');
		break;
		case 3:
		LCD->WriteData('3');
		break;
		case 4:
		LCD->WriteData('4');
		break;
		case 5:
		LCD->WriteData('5');
		break;
		case 6:
		LCD->WriteData('6');
		break;
		case 7:
		LCD->WriteData('7');
		break;
		case 8:
		LCD->WriteData('8');
		break;
		case 9:
		LCD->WriteData('9');
		break;
		case 10:
		LCD->WriteData('A');
		break;
		case 11:
		LCD->WriteData('B');
		break;
		case 12:
		LCD->WriteData('C');
		break;
		case 13:
		LCD->WriteData('D');
		break;
		case 14:
		LCD->WriteData('E');
		break;
		case 15:
		LCD->WriteData('F');
		break;
	}
}