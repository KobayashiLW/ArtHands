#include "InputManager.h"

InputManager::InputManager()
{
	bIsMoveFirstProcess = true;
}

InputManager::~InputManager()
{

}

LRESULT InputManager::MsgProc(HWND Hwnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
	switch (Msg)
	{
	case WM_LBUTTONDOWN:
		bIsMouseLButtonDown = true;
		X = GET_X_LPARAM(lParam);
		Y = GET_Y_LPARAM(lParam);
		break;
	case WM_LBUTTONUP:
		bIsMouseLButtonDown = false;
		break;
	case WM_RBUTTONDOWN:
		bIsMouseRButtonDown = true;
		break;
	case WM_RBUTTONUP:
		bIsMouseRButtonDown = false;
		break;
	case WM_MOUSEMOVE:
		if (bIsMouseLButtonDown)
		{
			if (bIsMoveFirstProcess)
			{
				bIsMoveFirstProcess = false;
				LastOffsetX = X;
				LastOffsetY = Y;
			}
			
			OffsetX = GET_X_LPARAM(lParam) - LastOffsetX;
			OffsetY = GET_Y_LPARAM(lParam) - LastOffsetY;

			LastOffsetX = OffsetX;
			LastOffsetY = OffsetY;
		}
		break;
	}

	return S_OK;
}
