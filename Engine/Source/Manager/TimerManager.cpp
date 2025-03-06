#include "TimerManager.h"

bool TimerManager::Init()
{
    LOG("Ÿ�̸� �Ŵ��� �ʱ�ȭ");

	//Ÿ�̸��� ���ļ�(�ʴ� ������)�� ��� �Լ�
	//https://learn.microsoft.com/ko-kr/windows/win32/api/profileapi/nf-profileapi-queryperformancefrequency
    BOOL result = QueryPerformanceFrequency(reinterpret_cast<LARGE_INTEGER*>(&frequency));
	CHECK(result, "���ļ� ȹ�� ����", false);

	//Ÿ�̸��� CPU Ŭ������ ��� �Լ�
	//https://learn.microsoft.com/ko-kr/windows/win32/api/profileapi/nf-profileapi-queryperformancecounter
    result = QueryPerformanceCounter(reinterpret_cast<LARGE_INTEGER*>(&prevCount));
    CHECK(result, "CPU Ŭ���� ȹ�� ����", false);

    LOG("Ÿ�̸� �Ŵ��� �ʱ�ȭ ����");
	return true;
}

float TimerManager::Update()
{
	//���κ��忡 �����ϴ� ���ػ��� Ÿ�̸Ӹ� �̿���
	//Ư�� ���� �������� CPU Ŭ�������� ���� ��
	//�� ���̸� �̿��Ͽ� �۾� �ð��� ���Ѵ�
	uint64 currentCount{};
	WARNING(QueryPerformanceCounter(reinterpret_cast<LARGE_INTEGER*>(&currentCount)), "CPU Ŭ���� ȹ�� ����");

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
