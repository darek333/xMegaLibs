
#include <avr/io.h>
#include <stdint-gcc.h>

class timerCounterN
{
	private:
	volatile uint8_t *TCCRA;
	volatile uint8_t *TCCRB;
	volatile uint8_t *TIFR;
	volatile uint8_t *TCNT;
	volatile uint8_t *OCRA;
	volatile uint8_t *OCRB;
	volatile uint8_t *TIMSK;
	
	uint8_t prescalerCalc(long t_delay_us);
	
	public:
	volatile bool counting;
	timerCounterN(volatile uint8_t *_TCCRA, volatile uint8_t *_TIMSK);
	void delay_us(long t_delay_us);
	void stopCounting();
};