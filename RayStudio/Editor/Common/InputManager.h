#pragma once
#include"windows.h"
#include"../../Core/Math/MathUtility.h"
#include "../../Core/Vector2D.h"

#define GET_X_LPARAM(lp) ((int16)(short)LOWORD(lp)) 
#define GET_Y_LPARAM(lp) ((int16)(short)HIWORD(lp))

class InputManager
{
public:
	bool bIsMouseLButtonDown;
	bool bIsMouseRButtonDown;

	int16 X;
	int16 Y;
	int16 OffsetX;
	int16 OffsetY;
	int16 LastOffsetX;
	int16 LastOffsetY;
	bool bIsMoveFirstProcess;
public:
	InputManager();
	~InputManager();

	void Update();

	LRESULT MsgProc(HWND Hwnd, UINT Msg, WPARAM wParam, LPARAM lParam);
};

