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

	float			getDeltaTime();
	float			getGlobalTime();
	float			getTimeScale();
	void			setTimeScale(float value);
private:
	INT64			_frequency;
	float			_ticksPerMs;
	INT64			_startTime;
	float			_frameTime;

	float			_timeScale;
	float			_globalTimer;
};
