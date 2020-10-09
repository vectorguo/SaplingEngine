#include "Time.h"
#include <Windows.h>

int64_t Time::s_BaseTime = 0;
int64_t Time::s_PausedTime = 0;
int64_t Time::s_StopTime = 0;
int64_t Time::s_CurrentTime = 0;
int64_t Time::s_PreviousTime = 0;
double Time::s_SecondPreCount = 0;
double Time::s_DeltaTime = 0;
bool Time::s_Stopped = false;

float Time::DeltaTime()
{
	return static_cast<float>(s_DeltaTime);
}

float Time::TotalTime()
{
	if (s_Stopped)
	{
		return static_cast<float>(static_cast<double>(s_StopTime - s_PausedTime - s_BaseTime) * s_SecondPreCount);
	}
	else
	{
		return static_cast<float>(static_cast<double>(s_CurrentTime - s_PausedTime - s_BaseTime) * s_SecondPreCount);
	}
}

void Time::Initialize()
{
	int64_t countsPerSecond;
	QueryPerformanceFrequency(reinterpret_cast<LARGE_INTEGER*>(&countsPerSecond));
	s_SecondPreCount = 1.0 / static_cast<double>(countsPerSecond);

	Reset();
}

void Time::Reset()
{
	QueryPerformanceCounter(reinterpret_cast<LARGE_INTEGER*>(&s_BaseTime));

	s_PreviousTime = s_BaseTime;
	s_StopTime = 0;
	s_Stopped = false;
}

void Time::Start()
{
	if (!s_Stopped)
	{
		return;
	}

	int64_t startTime;
	QueryPerformanceCounter(reinterpret_cast<LARGE_INTEGER*>(&startTime));
	
	s_PausedTime += startTime - s_StopTime;
	s_PreviousTime = startTime;
	s_StopTime = 0;
	s_Stopped = false;
}

void Time::Stop()
{
	if (s_Stopped)
	{
		return;
	}

	QueryPerformanceCounter(reinterpret_cast<LARGE_INTEGER*>(&s_StopTime));
	s_Stopped = true;
}

void Time::Tick()
{
	if (s_Stopped)
	{
		s_DeltaTime = 0;
	}
	else
	{
		QueryPerformanceCounter(reinterpret_cast<LARGE_INTEGER*>(&s_CurrentTime));

		s_DeltaTime = static_cast<double>(s_CurrentTime - s_PreviousTime) * s_SecondPreCount;
		if (s_DeltaTime < 0)
		{
			s_DeltaTime = 0;
		}
		s_PreviousTime = s_CurrentTime;
	}
}