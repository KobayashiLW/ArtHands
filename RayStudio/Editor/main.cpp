
#include <imgui.h>
#include <d3d11.h>
#include "Common\HUD.h"
#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>
#include <tchar.h>

#include "Common\DeviceManager.h"

static DeviceManager *gptr_device_manager_;
static HUD *gptr_hud_;
//AppFramement	*gptr_app_ = nullptr;
//
//int WINAPI WinMain(HINSTANCE hinstance,
//	HINSTANCE prevInstance,
//	PSTR cmdLine,
//	int showCmd)
//{
//
//	gptr_app_ = new AppFramement();
//	gptr_app_->Create(prevInstance, 1280, 720, "RayStudio");
//
//	string path = "H:\\sponza.FBX";
//	gptr_app_->ResManager.LoadResource(&path);
//
//	gptr_app_->Run();
//
//	return 0;
//}

extern LRESULT ImGui_ImplDX11_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if (ImGui_ImplDX11_WndProcHandler(hWnd, msg, wParam, lParam))
		return true;

	switch (msg)
	{
	case WM_SIZE:
		if (gptr_device_manager_ ->gptr_d3d_device_ != NULL && wParam != SIZE_MINIMIZED)
		{
			ImGui_ImplDX11_InvalidateDeviceObjects();
			gptr_device_manager_->CleanupRenderTarget(gptr_device_manager_->gptr_main_render_target_view_);
			gptr_device_manager_->gptr_swap_chain_->ResizeBuffers(0, (UINT)LOWORD(lParam), (UINT)HIWORD(lParam), DXGI_FORMAT_UNKNOWN, 0);
			gptr_device_manager_->CreateRenderTarget(gptr_device_manager_->gptr_main_render_target_view_);
			ImGui_ImplDX11_CreateDeviceObjects();
		}
		return 0;
	case WM_SYSCOMMAND:
		if ((wParam & 0xfff0) == SC_KEYMENU) // Disable ALT application menu
			return 0;
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return DefWindowProc(hWnd, msg, wParam, lParam);
}

int main(int, char**)
{
	// Create application window
	WNDCLASSEX wc = { sizeof(WNDCLASSEX), CS_CLASSDC, WndProc, 0L, 0L, GetModuleHandle(NULL), NULL, LoadCursor(NULL, IDC_ARROW), NULL, NULL, _T("Ray Studio"), NULL };
	RegisterClassEx(&wc);
	HWND hwnd = CreateWindow(_T("Ray Studio"), _T("Ray Studio"), WS_OVERLAPPEDWINDOW, 100, 100, 1366, 768, NULL, NULL, wc.hInstance, NULL);
	
	gptr_device_manager_ = new DeviceManager();
	gptr_hud_ = new HUD();

	// Initialize Direct3D
	if (gptr_device_manager_->CreateDeviceD3D(hwnd) < 0)
	{
		gptr_device_manager_->CleanupDeviceD3D();
		UnregisterClass(_T("Ray Studio"), wc.hInstance);
		return 1;
	}
	
	// Show the window
	ShowWindow(hwnd, SW_SHOWDEFAULT);
	UpdateWindow(hwnd);

	// Setup ImGui binding
	ImGui_ImplDX11_Init(hwnd, gptr_device_manager_->gptr_d3d_device_, gptr_device_manager_->gptr_d3d_device_context_);


	bool show_test_window = true;
	bool show_another_window = false;
	ImVec4 clear_col = ImColor(114, 144, 154);

	// Main loop
	MSG msg;
	ZeroMemory(&msg, sizeof(msg));
	while (msg.message != WM_QUIT)
	{
		if (PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
			continue;
		}

		ImGui_ImplDX11_NewFrame();

		//// 1. Show a simple window
		//// Tip: if we don't call ImGui::Begin()/ImGui::End() the widgets appears in a window automatically called "Debug"
		//{
		//	static float f = 0.0f;
		//	ImGui::Text("Hello, world!");
		//	ImGui::SliderFloat("float", &f, 0.0f, 1.0f);
		//	ImGui::ColorEdit3("clear color", (float*)&clear_col);
		//	if (ImGui::Button("Test Window")) show_test_window ^= 1;
		//	if (ImGui::Button("Another Window")) show_another_window ^= 1;
		//	ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		//}
		//// 2. Show another simple window, this time using an explicit Begin/End pair
		//if (show_another_window)
		//{
		//	ImGui::Begin("Another Window", &show_another_window);
		//	ImGui::Text("Hello from another window!");
		//	ImGui::Button("123");
		//	ImGui::End();
		//}

		//// 3. Show the ImGui test window. Most of the sample code is in ImGui::ShowTestWindow()
		//if (show_test_window)
		//{
		//	ImGui::SetNextWindowPos(ImVec2(650, 20), ImGuiCond_FirstUseEver);     // Normally user code doesn't need/want to call it because positions are saved in .ini file anyway. Here we just want to make the demo initial state a bit more friendly!
		//	ImGui::ShowTestWindow(&show_test_window);
		//}

		gptr_hud_->ShowMainMenu();
		gptr_hud_->ShowSceneEditWindow();
		gptr_hud_->ShowDetailsWindow();

		// Rendering
		gptr_device_manager_->gptr_d3d_device_context_->ClearRenderTargetView(gptr_device_manager_->gptr_main_render_target_view_, (float*)&clear_col);
		ImGui::Render();



		gptr_device_manager_->gptr_swap_chain_->Present(1, 0); // Present with vsync
	}

	ImGui_ImplDX11_Shutdown();

	gptr_device_manager_->CleanupDeviceD3D();
	UnregisterClass(_T("Ray Studio"), wc.hInstance);

	return 0;
}