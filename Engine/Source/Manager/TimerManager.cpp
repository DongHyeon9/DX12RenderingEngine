#include "TimerManager.h"

bool TimerManager::Init()
{
    LOG("타이머 매니저 초기화");

	//타이머의 주파수(초당 진동수)를 얻는 함수
	//https://learn.microsoft.com/ko-kr/windows/win32/api/profileapi/nf-profileapi-queryperformancefrequency
    BOOL result = QueryPerformanceFrequency(reinterpret_cast<LARGE_INTEGER*>(&frequency));
	CHECK(result, "주파수 획득 실패", false);

	//타이머의 CPU 클럭수를 얻는 함수
	//https://learn.microsoft.com/ko-kr/windows/win32/api/profileapi/nf-profileapi-queryperformancecounter
    result = QueryPerformanceCounter(reinterpret_cast<LARGE_INTEGER*>(&prevCount));
    CHECK(result, "CPU 클럭수 획득 실패", false);

    LOG("타이머 매니저 초기화 성공");
	return true;
}

float TimerManager::Update()
{
	//메인보드에 존재하는 고해상도의 타이머를 이용해
	//특정 실행 시점들의 CPU 클럭수들을 얻어온 후
	//그 차이를 이용하여 작업 시간을 구한다
	uint64 currentCount{};
	WARNING(QueryPerformanceCounter(reinterpret_cast<LARGE_INTEGER*>(&currentCount)), "CPU 클럭수 획득 실패");

	deltaTime = (currentCount - prevCount) / static_cast<float>(frequency);
	prevCount = currentCount;

	frameCount++;
	frameTime += deltaTime;

	if (frameTime > 1.f)
	{
		fps = static_cast<uint32>(frameCount / frameTime);

		frameTime = 0.f;
		frameCount = 0;
	}

	return deltaTime;
}
