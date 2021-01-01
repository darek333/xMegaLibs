#include "timerCounterN.h"

//Na spokojnie o protestach z Moniką Jaruzelską od 40min. 44:30

timerCounterN::timerCounterN(volatile uint8_t *_TCCRA, volatile uint8_t *_TIMSK)
{
	TCCRA = _TCCRA;
	TCCRB = _TCCRA+1;
	TCNT = _TCCRA+2;
	OCRA = _TCCRA+3;
	OCRB = _TCCRA+4;
	TIMSK = _TIMSK;
	
	*TCNT=0;
	*TCCRA = 0x02; //Set TCNT mode
	*TIMSK = 0x02; //Enable interrupt when reaching top value 
	counting= false;
}

void  timerCounterN::delay_us(long t_delay_us)
{
	uint8_t registerValue = 0xff;
	uint8_t multiplier = F_CPU/1000000;
	long stepsOfClock = t_delay_us*multiplier;
	uint8_t prescaler = prescalerCalc(stepsOfClock);
	
	
	registerValue = (uint8_t)((stepsOfClock)>>(prescaler-1));
	*TCNT=0;
	counting = true;
	*OCRA = registerValue;
	*TCCRB = 5;
}

void timerCounterN::stopCounting()
{
	*TCCRB = 0;
	*TCNT=0;
	counting = false;
}

uint8_t timerCounterN::prescalerCalc(long t_delay_us)
{
	uint8_t prescaler = 1;
	if (t_delay_us>0xff)
	{
		prescaler = prescalerCalc(t_delay_us>>1);
		prescaler++;
	}
	return prescaler;
}