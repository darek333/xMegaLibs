/*
Made by: Darek
26.10.2020
*/
#include "InputOutput.h"
#include "LinearModule.h"

Reciprocating_motions::Reciprocating_motions(long _lengthInSteps, int _forwardFrequency, int _backwardFrequency, PORT_struct *_DirectionPort, int _directionPin, PORT_struct *_StepPort, int _stepPin, PORT_struct *_TriggerCameraPort, uint8_t _triggerCameraPin, TC0_struct * _TimerCounter)
{
	lengthInSteps = _lengthInSteps;
	forwardFrequency = _forwardFrequency;
	backwardFrequency = _backwardFrequency;
	movingDirection = false;
	DirectionPort = _DirectionPort;
	directionPin = _directionPin;
	StepPort = _StepPort;
	stepPin = _stepPin;
	stepsDone = 0;
	halfLoopsDone = 0;
	TriggerCameraPort = _TriggerCameraPort;
	triggerCameraPin = _triggerCameraPin;
	firstStraight = true;
	TimerCounter=_TimerCounter;
	TimerCounter->CTRLB=0; //Set normal operating mode
	TimerCounter->INTCTRLA=1;//Enable and set low level priority for overflow/underflow interrupt 
	TimerCounter->PERH=0xff; //Higher bits of period register (top value in period)
	TimerCounter->PERL=0xff; //Lower bits of period register (top value in period)
	TimerCounter->CTRLFCLR = 1; //Set counter for up counting mode. 
	TimerCounter->CTRLFCLR = 1<<1; //Enable update of the buffered registers
	DirectionPort->DIRSET=1<<_directionPin; //Change movement direction pin for output
	StepPort->DIRSET=1<<_stepPin; //Change step pin for output
	TriggerCameraPort->DIRSET=1<<triggerCameraPin; //Change camera trigger pin to output
}

Reciprocating_motions::~Reciprocating_motions()
{
	TimerCounter->CTRLA=0; //Stops timer/counter
	TimerCounter->INTCTRLA = 0; //Disable overflow/underflow interrupt
	TimerCounter->CTRLFCLR= 0b00000010<<2; //Clear Timer counter register
	TimerCounter->PERH = 0; //Higher bits of period register (top value in period)
	TimerCounter->PERL = 0; //Lower bits of period register (top value in period)
	DirectionPort->DIRCLR=1<<directionPin; //Set pin as input
	StepPort->DIRCLR=1<<stepPin; //Set pin as input
	TriggerCameraPort->DIRCLR=1<<triggerCameraPin; //Set pin as input
} 

void Reciprocating_motions::overflowInterrupt ()
{
	StepPort->OUTTGL=1<<stepPin; // Make step
	
	if (!firstStraight)
	{
		stepsDone +=1; //increment number of steps that has been done
		if (!movingDirection)
		{
			StepsToTriggerDone+=1; //increment number of steps to trigger
		}
	}
	
	if (StepsToTriggerDone==(StepsToTrigger-cameraTriggerDuration))
	{
		TriggerCameraPort->OUTSET=1<<triggerCameraPin;
	}
	if (StepsToTriggerDone==StepsToTrigger)
	{
		TriggerCameraPort->OUTCLR=1<<triggerCameraPin;
		StepsToTriggerDone=0;
	}
	
	if (stepsDone==lengthInSteps) //If done all steps in this half of loop, than change direction and count again. 
	{
		halfLoopsDone+=1;
		stepsDone=0;
		DirectionPort->OUTTGL=1<<directionPin;
		movingDirection^=true;
		if (movingDirection)
		{
			changeFrequency(backwardPeriodH, backwardPeriodL, backwardPrescaler);
			StepsToTriggerDone=0;
		}
		else
		{
			changeFrequency(forwardPeriodH, forwardPeriodL, forwardPrescaler);
		}
	}
	/*if (halfLoopsDone==2*loopsToDo)
	{
		TCC0.CTRLA=0; //Stops timer/counter
		TCC0.CTRLFCLR= 0b00000010<<2; //Clear Timer counter register 
	}*/
}
	
void Reciprocating_motions::reciprocating_motion_init (int _StepsToTrigger, uint8_t _cameraTriggerDuration, int _loopsToDo, bool _beginningDirection, int _forwardFrequency, int _backwardFrequency)
{
	uint8_t tableOfSettings[3];
	firstStraight = true;
	loopsToDo=_loopsToDo;
	movingDirection=_beginningDirection;
	StepsToTrigger = _StepsToTrigger;
	cameraTriggerDuration = _cameraTriggerDuration;
	StepsToTriggerDone=0;
	if (_beginningDirection==true) //Change direction of moving
	{
		DirectionPort->OUTSET=1<<directionPin;
	}
	else if (_beginningDirection==false)
	{
		DirectionPort->OUTCLR=1<<directionPin;
	}
	
	TimerCounter->CTRLE=0; //Chose counting direction
	
	if (_forwardFrequency!=0) //Change forward stepping frequency is needed
	{
		forwardFrequency = _forwardFrequency;
	}
	calculateRegister(32000000, forwardFrequency, tableOfSettings);
	forwardPeriodH = tableOfSettings[2];
	forwardPeriodL = tableOfSettings[1];
	forwardPrescaler = tableOfSettings[0];
	
	if (_backwardFrequency!=0) //Change backward stepping frequency is needed
	{
		backwardFrequency = _backwardFrequency;
	}
	calculateRegister(32000000, backwardFrequency, tableOfSettings);
	backwardPeriodH = tableOfSettings[2];
	backwardPeriodL = tableOfSettings[1];
	backwardPrescaler = tableOfSettings[0];
	
	if (movingDirection)
	{
		changeFrequency(backwardPeriodH, backwardPeriodL, backwardPrescaler);
	} 
	else
	{
		changeFrequency(forwardPeriodH, forwardPeriodL, forwardPrescaler);
	}
	
}

void Reciprocating_motions::changeFrequency(uint8_t highByte, uint8_t lowByte, uint8_t prescaler) //function change timer counter frequency
{
	if (((TimerCounter->CTRLA)&0x0f)!=0) //Check if timer counter is working. If it does, it must be stop.
	{
		TimerCounter->CTRLA = 0;
	}
	TimerCounter->PERH = highByte; //Higher bits of period register (top value in period)
	TimerCounter->PERL = lowByte; //Lower bits of period register (top value in period)
	TimerCounter->CTRLA = prescaler; //Set prescaler and begin counting
}

void Reciprocating_motions::calculateRegister (long cpuFrequncy, long expectedFrequency, uint8_t*tableOfResults) //function that calculate value of timer/counter register which determinate frequency in normal mode
{
	uint8_t prescaler = 0;
	long stepsNumber; 
	prescaler=checkPrescaler(cpuFrequncy, expectedFrequency);
	if(prescaler>0)
	{
		stepsNumber=(cpuFrequncy/prescaler)/expectedFrequency;
	}
	else
	{
		stepsNumber=cpuFrequncy/expectedFrequency;
	}
	tableOfResults[0] = prescaler;
	tableOfResults[1] = (uint8_t)(stepsNumber&0xff);
	tableOfResults[2] = (uint8_t)(stepsNumber>>8);
}

uint8_t Reciprocating_motions::checkPrescaler(long cpuFrequncy, long expectedFrequency) //Function calculate prescaler for expected frequency with set clock frequency
{
	uint8_t prescaler = 1;
	long stepsNumber = cpuFrequncy/expectedFrequency;
	if (stepsNumber>0xffff)
	{
		prescaler=checkPrescaler(cpuFrequncy/2, expectedFrequency);
		prescaler = prescaler+1;
	}
	return prescaler;
}

void Reciprocating_motions::stop()
{
	TimerCounter->CTRLA = 0; //Reset prescaler and stop counting
}

void Reciprocating_motions::resume()
{
	if (movingDirection)
	{
		TimerCounter->CTRLA= backwardPrescaler; //Set prescaler and begin counting
	} 
	else
	{
		TimerCounter->CTRLA = forwardPrescaler; //Set prescaler and begin counting
	}
}

void Reciprocating_motions::changeDirection()
{
	stop();
	stepsDone=lengthInSteps-stepsDone;
	DirectionPort->OUTTGL=1<<directionPin;
	movingDirection^=true;
	resume();
}

void Reciprocating_motions::calibrationEnd()
{
	firstStraight=false;
	DirectionPort->OUTTGL=1<<directionPin;
	changeFrequency(forwardPeriodH, forwardPeriodL, forwardPrescaler);
	movingDirection^=true;
}