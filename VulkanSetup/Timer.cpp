#include "Timer.h"

Timer::Timer()
:_frequency(0),_ticksPerMs(0),_startTime(0),_frameTime(0),_timeScale(0),_globalTimer(0)

{
}

Timer::~Timer()
{
}

Timer* Timer::getInstance()
{
	static Timer timer;
	return &timer;
}

bool Timer::initialize()
{
	QueryPerformanceFrequency((LARGE_INTEGER*)&_frequency);
	if (_frequency == 0)
	{
		return false;
	}

	_ticksPerMs = (float)(_frequency);

	QueryPerformanceCounter((LARGE_INTEGER*)&_startTime);

	_timeScale = 1.0;

	return true;
}

void Timer::frame()
{
	INT64 currentTime = 0;
	QueryPerformanceCounter((LARGE_INTEGER*)&currentTime);

	float timeDifference = (float)(currentTime - _startTime);
	_frameTime = timeDifference / _ticksPerMs;
	_startTime = currentTime;

	_globalTimer += _frameTime;
}

float Timer::getDeltaTime()
{
	return _frameTime * _timeScale;
}

float Timer::getGlobalTime()
{
	return _globalTimer;
}

float Timer::getTimeScale()
{
	return _timeScale;
}

void Timer::setTimeScale(float value)
{
	_timeScale = value;
}
