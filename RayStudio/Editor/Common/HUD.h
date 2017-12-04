#pragma once
#include <imgui.h>
#include<vector>
#include "../../Core/Scene/Actor.h"

using std::vector;

struct ID3D11Device;
struct ID3D11DeviceContext;

IMGUI_API bool        ImGui_ImplDX11_Init(void* hwnd, ID3D11Device* device, ID3D11DeviceContext* device_context);
IMGUI_API void        ImGui_ImplDX11_Shutdown();
IMGUI_API void        ImGui_ImplDX11_NewFrame();

// Use if you want to reset your rendering device without losing ImGui state.
IMGUI_API void        ImGui_ImplDX11_InvalidateDeviceObjects();
IMGUI_API bool        ImGui_ImplDX11_CreateDeviceObjects();


class HUD
{
public:
	bool bIsShowMainMenu;
	bool bIsShowStyleEditor;
	bool bIsShowDetailsWindow;
	bool bIsShowSceneEditWindow;

	ImGuiStyle GuiStyle;
	ImGuiStyle GuiStyleDefault;


	std::vector<std::string> _langs;

private:
	Actor *ptr_select_object_;

	ImVec4 ViewPort;

public:
	HUD();
	~HUD();

	void Render();

	void ShowMainMenu();
	void ShowStyleEditor();
	void ShowDetailsWindow();
	void ShowActorTransformWindow(Actor *InActor);
	void ShowSceneEditWindow();

private:
	void SetStyle(ImGuiStyle &NewStyle);
};

