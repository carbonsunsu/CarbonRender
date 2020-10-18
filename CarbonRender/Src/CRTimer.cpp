#include "..\Inc\CRTimer.h"

Timer* Timer::ins = nullptr;

Timer::Timer()
{
	timerIndex = 0;
}

Timer::~Timer()
{
}

Timer * Timer::Instance()
{
	if (ins == nullptr)
		ins = new Timer();

	return ins;
}

void Timer::UpdateTimer(unsigned int timerID)
{
	if (timers.find(timerID) != timers.end())
		timers[timerID] = glfwGetTime();
}

unsigned int Timer::NewTimer()
{
	timers[timerIndex] = glfwGetTime();
	return timerIndex++;
}

void Timer::RemoveTimer(unsigned int timerID)
{
	if (timers.find(timerID) != timers.end())
		timers.erase(timerID);
}

double Timer::LastUpdateTime(unsigned int timerID)
{
	if (timers.find(timerID) != timers.end())
		return timers[timerID];
	else
		return 0.0;
}

double Timer::GetTime()
{
	return glfwGetTime();
}
