#include "Timer.h"

Timer::Timer()
	:_frequency(0),_ticksPerMs(0), _startTime(0), _frameTime(0), _timeScale(1.f)
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

	_ticksPerMs = (double)(_frequency);

	QueryPerformanceCounter((LARGE_INTEGER*)&_startTime);

	_timeScale = 1.0;

	return true;
}

void Timer::frame()
{
	INT64 currentTime = 0;
	QueryPerformanceCounter((LARGE_INTEGER*)&currentTime);

	double timeDifference = (double)(currentTime - _startTime);
	_frameTime = timeDifference / _ticksPerMs;
	_startTime = currentTime;

	_globalTimer += _frameTime;
}

double Timer::getTime()
{
	return _frameTime * _timeScale;
}

double Timer::getGlobalTime()
{
	return _globalTimer;
}

double Timer::getTimeScale()
{
	return _timeScale;
}

void Timer::setTimeScale(double value)
{
	_timeScale = value;
}
