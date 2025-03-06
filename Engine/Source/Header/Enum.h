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
	T0 = 0,
	T1,
	T2,
	T3,
	T4,

	END
};

enum class E_UAV_REGISTER : uint8
{
	U0 = 0,
	U1,
	U2,
	U3,
	U4,

	END
};

enum class E_TABLE_CBV_REGISTER : uint8
{
	B5 = E_CBV_REGISTER::END,
	B6,
	B7,
	B8,
	B9,
	B10,
	B11,
	B12,
	B13,
	B14,

	END
};

enum class E_TABLE_SRV_REGISTER : uint8
{
	T5 = E_SRV_REGISTER::END,
	T6,
	T7,
	T8,
	T9,
	T10,
	T11,
	T12,
	T13,
	T14,

	END
};

enum class E_TABLE_UAV_REGISTER : uint8
{
	U5 = E_UAV_REGISTER::END,
	U6,
	U7,
	U8,
	U9,
	U10,
	U11,
	U12,
	U13,
	U14,

	END
};