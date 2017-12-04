#include "AppFramement.h"
#include "DeviceManager.h"
#include <tchar.h>
//extern AppFramement	*gptr_app_;
AppFramement	*gptr_app_;

AppFramement::AppFramement()
{
}


AppFramement::~AppFramement()
{
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

BOOL AppFramement::Create(HINSTANCE hInstance, int Width, int Height, LPCSTR LpszWndName)
{
	WNDCLASS wc;
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = (WNDPROC)WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInstance;
	wc.hIcon = LoadIcon(0, IDI_APPLICATION);
	wc.hCursor = LoadCursor(0, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wc.lpszMenuName = 0;
	wc.lpszClassName = _T("RayStudio");

	if (!RegisterClass(&wc))
	{
		//::MessageBox(0, "Register Window Fail", 0, 0);
		return false;
	}

	LONG cpSzie = GetSystemMetrics(SM_CYCAPTION);
	LONG xdlgSzie = GetSystemMetrics(SM_CXDLGFRAME);
	LONG ydlgSzie = GetSystemMetrics(SM_CXDLGFRAME);
	LONG exW = xdlgSzie * 2 + Width;
	LONG exH = ydlgSzie * 2 + cpSzie + Height;
	LONG scSzieW = GetSystemMetrics(SM_CXSCREEN);
	LONG scSzieH = GetSystemMetrics(SM_CYSCREEN);
	LONG PosX = (scSzieW - exW) / 2;
	LONG PosY = (scSzieH - exH) / 2 - cpSzie;
	
	InfoStreams.HWnd = CreateWindow(_T("Ray Studio"), NULL, WS_CLIPCHILDREN | WS_OVERLAPPEDWINDOW,
		PosX, PosY, exW, exH, NULL, LoadMenu(hInstance, MAKEINTRESOURCE(NULL)), hInstance, NULL);

	if (!InfoStreams.HWnd)
	{
		//::MessageBox(0, "Create Window Fail", 0, 0);
		return false;
	}
	
	InfoStreams.WndWidth = Width;
	InfoStreams.WndHeight = Height;

	::ShowWindow(InfoStreams.HWnd, SW_SHOW);
	::UpdateWindow(InfoStreams.HWnd);

	//Create device and data
	DeviceStreams.Init(InfoStreams);
	DeviceStreams.CreateD3d11Device();


	ResManager.Init(&DeviceStreams, &SceneLevel);

	return TRUE;
}


LRESULT CALLBACK AppFramement::MsgProc(HWND Hwnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
	switch (Msg)
	{
	case WM_DESTROY:
		::PostQuitMessage(0);
		break;

	case WM_KEYDOWN:
		if (wParam == VK_ESCAPE)
			::DestroyWindow(Hwnd);
		break;
	case WM_ACTIVATE:

		break;
	case WM_LBUTTONDOWN:

		break;
	case WM_LBUTTONUP:

		break;

	default:
		return DefWindowProc(Hwnd, Msg, wParam, lParam);
	}
	InputStreams.MsgProc(Hwnd, Msg, wParam, lParam);


	return 0;
}

WPARAM AppFramement::Run()
{
	MSG msg;
	::ZeroMemory(&msg, sizeof(MSG));

	while (msg.message != WM_QUIT)
	{
		if (::PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
		{
			::TranslateMessage(&msg);
			::DispatchMessage(&msg);
		}
		// update fps information
		InfoStreams.UpdateFPS();


		//设置清除缓存的颜色  
		float aColor[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
		//清除缓存  
		
		DeviceStreams.ptr_device_context_->ClearRenderTargetView(DeviceStreams.ptr_render_target_view_, aColor);
		//清除深度缓存和模板缓存  
		DeviceStreams.ptr_device_context_->ClearDepthStencilView(DeviceStreams.ptr_depth_stencil_view_, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

		//提交更新
		DeviceStreams.ptr_swap_chain_->Present(0, 0);
	}
	return int(msg.wParam);

}







//LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
//{
//	return gptr_app_->MsgProc(hWnd, uMsg, wParam, lParam);
//}