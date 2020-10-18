#ifndef CR_TIMER
#define CR_TIMER

#include "..\Inc\CRGloble.h"

class Timer
{
private:
	static Timer* ins;
	Timer();
	unordered_map<unsigned int, double> timers;
	unsigned int timerIndex;

public:
	~Timer();
	static Timer* Instance();
	void UpdateTimer(unsigned int timerID);
	unsigned int NewTimer();
	void RemoveTimer(unsigned int timerID);
	double LastUpdateTime(unsigned int timerID);
	double GetTime();

};

#endif 
