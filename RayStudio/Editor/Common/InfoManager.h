#pragma once
#include"windows.h"
#include"../../Core/Math/MathUtility.h"

class InfoManager
{

public:
	uint64 fps;


	//Window Info
	HINSTANCE	HWndHinstance;
	HWND		HWnd;
	int16 WndWidth;
	int16 WndHeight;


public:
	InfoManager();
	~InfoManager();

	void UpdateFPS();


	inline uint64 GetFps() { return fps; }
};

