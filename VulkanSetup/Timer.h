#pragma once
#include <windows.h>

class Timer
{
public:
					Timer();
					~Timer();

	static Timer*	getInstance();

	bool			initialize();
	void			frame();

	double			getTime();
	double			getGlobalTime();
	double			getTimeScale();
	void			setTimeScale(double value);
private:
	INT64			_frequency;
	double			_ticksPerMs;
	INT64			_startTime;
	double			_frameTime;

	double			_timeScale;
	double			_globalTimer;
};
