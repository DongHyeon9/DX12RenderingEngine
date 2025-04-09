#include "InputManager.h"
#include "Engine/Engine.h"

bool InputManager::Init()
{
	LOG("입력 매니저 초기화");


	LOG("입력 매니저 초기화 성공");
	return true;
}

void InputManager::Update()
{
	//현재 포커싱된 윈도우가 내 엔진이 아니라면 키상태 초기화
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

		// 키가 눌려 있으면 true
		if (asciiKeys[key] & 0x80)
		{
			// 이전 프레임에 키를 누른 상태라면 PRESS
			if (state == E_KEY_STATE::PRESS || state == E_KEY_STATE::DOWN)
				state = E_KEY_STATE::PRESS;
			else
				state = E_KEY_STATE::DOWN;
		}
		else
		{
			// 이전 프레임에 키를 누른 상태라면 UP
			if (state == E_KEY_STATE::PRESS || state == E_KEY_STATE::DOWN)
				state = E_KEY_STATE::UP;
			else
				state = E_KEY_STATE::NONE;
		}
	}

	GetCursorPos(&mousePos);
	ScreenToClient(engineWindowHandle, &mousePos);
}
