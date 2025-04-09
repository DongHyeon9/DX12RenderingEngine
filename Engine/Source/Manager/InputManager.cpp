#include "InputManager.h"
#include "Engine/Engine.h"

bool InputManager::Init()
{
	LOG("�Է� �Ŵ��� �ʱ�ȭ");


	LOG("�Է� �Ŵ��� �ʱ�ȭ ����");
	return true;
}

void InputManager::Update()
{
	//���� ��Ŀ�̵� �����찡 �� ������ �ƴ϶�� Ű���� �ʱ�ȭ
	HWND engineWindowHandle = GEngine->GetEngineSetting().hWnd;
	HWND currentWindowHandle = GetActiveWindow();

	if (engineWindowHandle != currentWindowHandle)
	{
		for (uint32 key = 0; key < KEY::TYPE_COUNT; ++key)
			states[key] = E_KEY_STATE::NONE;

		return;
	}

	BYTE asciiKeys[KEY::TYPE_COUNT]{};
	if (!GetKeyboardState(asciiKeys))
		return;

	for (uint32 key = 0; key < KEY::TYPE_COUNT; ++key)
	{
		E_KEY_STATE& state = states[key];

		// Ű�� ���� ������ true
		if (asciiKeys[key] & 0x80)
		{
			// ���� �����ӿ� Ű�� ���� ���¶�� PRESS
			if (state == E_KEY_STATE::PRESS || state == E_KEY_STATE::DOWN)
				state = E_KEY_STATE::PRESS;
			else
				state = E_KEY_STATE::DOWN;
		}
		else
		{
			// ���� �����ӿ� Ű�� ���� ���¶�� UP
			if (state == E_KEY_STATE::PRESS || state == E_KEY_STATE::DOWN)
				state = E_KEY_STATE::UP;
			else
				state = E_KEY_STATE::NONE;
		}
	}

	GetCursorPos(&mousePos);
	ScreenToClient(engineWindowHandle, &mousePos);
}
