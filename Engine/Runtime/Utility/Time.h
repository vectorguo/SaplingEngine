#pragma once

#include <cstdint>

class Time
{
public:
	inline static float DeltaTime()
	{
		return static_cast<float>(s_DeltaTime);
	}

	static float TotalTime();

	static void Initialize();
	static void Reset();
	static void Start();
	static void Stop();
	static void Tick();

private:
	static int64_t s_BaseTime;
	static int64_t s_PausedTime;
	static int64_t s_StopTime;
	static int64_t s_CurrentTime;
	static int64_t s_PreviousTime;

	static double s_SecondPreCount;
	static double s_DeltaTime;

	static bool s_Stopped;
};
