#pragma once
#include"windows.h"
#include"../../Core/Math/MathUtility.h"
#include "InfoManager.h"
#include "DeviceManager.h"
#include "../../Core/Scene/ResourceManager.h"
#include "../../Core/Scene/Level.h"
#include "../Common/InputManager.h"
#include "../../Core/Render/RenderEngine.h"

class AppFramement
{
public:

	InfoManager InfoStreams;
	InputManager InputStreams;
	DeviceManager DeviceStreams;
	ResourceManager ResManager;
	Level SceneLevel;
	RenderEngine Render;
public:
	
public:
	AppFramement();
	~AppFramement();

	BOOL Create(HINSTANCE hInstance, int Width, int Height, LPCSTR LpszWndName);
	virtual LRESULT CALLBACK MsgProc(HWND Hwnd, UINT Msg, WPARAM wParam, LPARAM lParam);

	WPARAM Run();
};

