#pragma once
#include "Header\EngineCore.h"

class TimerManager
{
	DECLARE_SINGLETON(TimerManager);
public:

protected:

private:
	std::chrono::system_clock::time_point lastTime{};
	std::chrono::system_clock::time_point currentTime{};
	std::chrono::duration<float> deltaTime{};

public:
	bool Init();
	float Update();

	FORCEINLINE uint32 GetFPS()const { return static_cast<uint32>(1 / deltaTime.count()); }
	FORCEINLINE float GetDeltaTime()const { return deltaTime.count(); }

protected:

private:

};

