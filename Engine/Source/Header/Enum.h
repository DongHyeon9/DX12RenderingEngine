#pragma once
#include "Type.h"
#include <Windows.h>

enum class E_KEY_TYPE : uint8
{
	UP = VK_UP,
	DOWN = VK_DOWN,
	LEFT = VK_LEFT,
	RIGHT = VK_RIGHT,

	KEY_1 = '1',
	KEY_2 = '2',
	KEY_3 = '3',
	KEY_4 = '4',
	KEY_5 = '5',
	KEY_6 = '6',
	KEY_7 = '7',
	KEY_8 = '8',
	KEY_9 = '9',
	KEY_0 = '0',

	F1 = VK_F1,
	F2 = VK_F2,
	F3 = VK_F3,
	F4 = VK_F4,
	F5 = VK_F5,
	F6 = VK_F6,
	F7 = VK_F7,
	F8 = VK_F8,
	F9 = VK_F9,
	F10 = VK_F10,
	F11 = VK_F11,
	F12 = VK_F12,

	Q = 'Q',
	W = 'W',
	E = 'E',
	R = 'R',
	T = 'T',
	Y = 'Y',
	U = 'U',
	I = 'I',
	O = 'O',
	P = 'P',

	A = 'A',
	S = 'S',
	D = 'D',
	F = 'F',
	G = 'G',
	H = 'H',
	J = 'J',
	K = 'K',
	L = 'L',

	Z = 'Z',
	X = 'X',
	C = 'C',
	V = 'V',
	B = 'B',
	N = 'N',
	M = 'M',

	SPACE = VK_SPACE,
	ENTER = VK_RETURN,
	ESCAPE = VK_ESCAPE,

	L_BUTTON = VK_LBUTTON,
	M_BUTTON = VK_MBUTTON,
	R_BUTTON = VK_RBUTTON,

	L_SHIFT = VK_LSHIFT,
	L_CTRL = VK_LCONTROL,
	L_ALT = VK_LMENU,

	R_SHIFT = VK_RSHIFT,
	R_CTRL = VK_RCONTROL,
	R_ALT = VK_RMENU,

	END,
};

enum class E_KEY_STATE : uint8
{
	NONE = 0,
	PRESS,
	DOWN,
	UP,

	END,
};

enum class E_CBV_REGISTER : uint8
{
	B0 = 0,
	B1,
	B2,
	B3,
	B4,

	END
};

enum class E_SRV_REGISTER : uint8
{
	T0 = static_cast<uint8>(E_CBV_REGISTER::END),
	T1,
	T2,
	T3,
	T4,
	T5,
	T6,
	T7,

	END
};

enum class E_CONSTANT_BUFFER_TYPE : uint8
{
	CAMERA = 0,
	LIGHT,
	NORMAL,
	TRANSFORM,
	MATERIAL,

	END
};

enum class E_LIGHT_TYPE : uint8
{
	POINT = 0,
	SPOT,
	DIRECTIONAL,

	END
};

enum class E_TEXTURE_TYPE : uint8
{
	ALBEDO = 0,
	NORMAL,
	METALLIC,
	ROUGHNESS,
	AMBIENT,
	HEIGHT,

	END
};

enum class E_RENDERING_FLAG : uint8
{
	DEFAULT = 0,

	END
};