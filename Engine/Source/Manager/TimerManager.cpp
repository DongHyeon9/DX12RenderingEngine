#include "TimerManager.h"

bool TimerManager::Init()
{
    LOG("Ÿ�̸� �Ŵ��� �ʱ�ȭ");

	lastTime = std::chrono::system_clock::now();

    LOG("Ÿ�̸� �Ŵ��� �ʱ�ȭ ����");
	return true;
}

float TimerManager::Update()
{
    currentTime = std::chrono::system_clock::now();
    deltaTime = currentTime - lastTime;
    lastTime = currentTime;
    return deltaTime.count();
}
