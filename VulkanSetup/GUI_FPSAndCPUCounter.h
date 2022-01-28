#pragma once
#include "GUIBase.h"
#include <Pdh.h>

class GUI_FPSAndCPUCounter : public GUIBase
{
public:
					GUI_FPSAndCPUCounter();
					GUI_FPSAndCPUCounter(const GUI_FPSAndCPUCounter&);
					~GUI_FPSAndCPUCounter();

	virtual void	initialize() override;
	virtual void	frame() override;
	virtual void	render() override;
private:
	int				GetCPUPercentage();
private:
	bool			_canReadCPU = true;
	HQUERY			_queryHandle = 0;
	HCOUNTER		_counterHandle = 0;
	ULONGLONG		_lastSampleTime = 0;
	long			_cpuUsage = 0;
};

