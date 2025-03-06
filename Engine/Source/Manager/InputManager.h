#pragma once
#include "Header\EngineCore.h"

class InputManager
{
	DECLARE_SINGLETON(InputManager);
public:

protected:

private:
	std::array<E_KEY_STATE, KEY::TYPE_COUNT> states{};
	POINT mousePos{};

public:
	bool Init();
	void Update();

	FORCEINLINE bool GetButtonPress(E_KEY_TYPE Key) { return GetState(Key) == E_KEY_STATE::PRESS; }
	FORCEINLINE bool GetButtonDown(E_KEY_TYPE Key) { return GetState(Key) == E_KEY_STATE::DOWN; }
	FORCEINLINE bool GetButtonUp(E_KEY_TYPE Key) { return GetState(Key) == E_KEY_STATE::UP; }
	FORCEINLINE const POINT& GetMousePos() { return mousePos; }

protected:

private:
	FORCEINLINE E_KEY_STATE GetState(E_KEY_TYPE Key) { return states[static_cast<uint8>(Key)]; }
};

