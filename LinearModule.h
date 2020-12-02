/*
Made by: Darek 
26.10.2020
*/

/*
	StepNumber - Number of step that gone be done before changing direction
	farwordFrequency - frequency of steps forward
	backwardFrequency - frequency of steps backward
	BeginningDirection - Direction when begin 
	DirectionPort - pointer for direction output port
	DirectionPin - number of direction output pin 
	StepPort - pointer for direction output port 
	StepPin - number of direction output pin
	*/
class Reciprocating_motions
{
	private:
	long stepsDone; //Number of steps already done
	int halfLoopsDone; //Number of loops already done
	int loopsToDo; //Number of loops that should be done
	int forwardPeriodH; //Number of clock steps for one step pin toggle in forward move high and low part
	int forwardPeriodL;
	int backwardPeriodH; //Number of clock steps for one step pin toggle in backward move high and low part
	int backwardPeriodL;
	uint8_t forwardPrescaler; // Prescaler value for forward and backward move
	uint8_t backwardPrescaler;
	int StepsToTrigger; //Number of steps to make between triggering single photo
	int StepsToTriggerDone;
	uint8_t cameraTriggerDuration; //Number of steps that determines camera trigger duration
	
	

	void changeFrequency(uint8_t highByte, uint8_t lowByte, uint8_t prescaler);
	void calculateRegister(long cpuFrequncy, long expectedFrequency, uint8_t*tableOfResults);
	uint8_t checkPrescaler(long cpuFrequncy, long expectedFrequency);

	public:
	long lengthInSteps; 
	int forwardFrequency;
	int backwardFrequency;
	bool movingDirection;
	PORT_struct *DirectionPort;
	int directionPin;
	PORT_struct *StepPort;
	int stepPin;
	PORT_struct *TriggerCameraPort;
	uint8_t triggerCameraPin;
	TC0_struct *TimerCounter;
	bool firstStraight;
	
	void reciprocating_motion_init (int _StepsToTrigger, uint8_t _cameraTriggerDuration, int _loopsToDo=0, bool _beginningDirection=false, int _forwardFrequency=0, int _backwardFrequency=0);
	void overflowInterrupt ();
	void stop();
	void resume();
	void changeDirection();
	void calibrationEnd();
	
	Reciprocating_motions(long _lengthInSteps, int _forwardFrequency, int _backwardFrequency, PORT_struct *_DirectionPort, int _directionPin, PORT_struct *_StepPort, int _stepPin, PORT_struct *_TriggerCameraPort, uint8_t _triggerCameraPin, TC0_struct * _TimerCounter);
	~Reciprocating_motions();
};


