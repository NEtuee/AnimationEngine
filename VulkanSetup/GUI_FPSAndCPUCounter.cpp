#include "GUI_FPSAndCPUCounter.h"
#include "Timer.h"
#include <windows.h>

GUI_FPSAndCPUCounter::GUI_FPSAndCPUCounter()
{
}

GUI_FPSAndCPUCounter::GUI_FPSAndCPUCounter(const GUI_FPSAndCPUCounter&)
{
}

GUI_FPSAndCPUCounter::~GUI_FPSAndCPUCounter()
{
	if (_canReadCPU)
	{
		PdhCloseQuery(_queryHandle);
	}
}

void GUI_FPSAndCPUCounter::initialize()
{
	PDH_STATUS status = PdhOpenQuery(NULL, 0, &_queryHandle);
	if (status != ERROR_SUCCESS)
	{
		_canReadCPU = false;
	}

	status = PdhAddCounter(_queryHandle, TEXT("\\Processor(_Total)\\% processor time"),0,&_counterHandle);
	if (status != ERROR_SUCCESS)
	{
		_canReadCPU = false;
	}

	_lastSampleTime = GetTickCount64();
	_cpuUsage = 0;
}

void GUI_FPSAndCPUCounter::frame()
{
	PDH_FMT_COUNTERVALUE value;

	if (_canReadCPU)
	{
		if ((_lastSampleTime + 1000) < GetTickCount64())
		{
			_lastSampleTime = GetTickCount64();
			PdhCollectQueryData((_queryHandle));
			PdhGetFormattedCounterValue(_counterHandle, PDH_FMT_LONG, NULL, &value);
			_cpuUsage = value.longValue;
		}
	}
}

void GUI_FPSAndCPUCounter::render()
{
	ImGui::SetNextWindowSize(ImVec2(200, 100), ImGuiSetCond_FirstUseEver);
	ImGui::Begin("System Info");
	ImGui::Text("%.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	ImGui::Text("CPU Usage %d %%", GetCPUPercentage());

	float timeScale = Timer::getInstance()->getTimeScale();
	ImGui::Text("time scale : "); ImGui::SameLine();
	ImGui::SliderFloat("##ts", &timeScale, 0.1f, 10.f);
	Timer::getInstance()->setTimeScale(timeScale);

	ImGui::End();
}

int GUI_FPSAndCPUCounter::GetCPUPercentage()
{
	int usage = 0;
	if (_canReadCPU)
		usage = (int)_cpuUsage;
	return usage;
}
