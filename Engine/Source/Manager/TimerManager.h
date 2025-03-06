#pragma once
#include "Header\EngineCore.h"

class TimerManager
{
	DECLARE_SINGLETON(TimerManager);
public:

protected:

private:
	uint64 frequency{};
	uint64 prevCount{};
	float deltaTime{};

	uint32 frameCount{};
	float frameTime{};
	uint32 fps{};

public:
	bool Init();
	float Update();

	FORCEINLINE uint32 GetFPS()const { return fps; }

protected:

private:

};

