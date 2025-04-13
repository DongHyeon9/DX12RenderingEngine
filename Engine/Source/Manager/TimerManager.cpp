#include "TimerManager.h"

bool TimerManager::Init()
{
    LOG("타이머 매니저 초기화");

	lastTime = std::chrono::system_clock::now();

    LOG("타이머 매니저 초기화 성공");
	return true;
}

float TimerManager::Update()
{
    currentTime = std::chrono::system_clock::now();
    deltaTime = currentTime - lastTime;
    lastTime = currentTime;
    return deltaTime.count();
}
