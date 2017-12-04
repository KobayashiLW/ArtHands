#pragma once
#include "imgui.h"

namespace ImGui
{
	 void ShutdownDock();
	 void RootDock(const ImVec2& pos, const ImVec2& size);
	 bool BeginDock(const char* label, bool* opened = nullptr, ImGuiWindowFlags extra_flags = 0, const ImVec2& default_size = ImVec2(-1, -1));
	 void EndDock();
	 void SetDockActive();
	 void LoadDock(const char* path);
	 void SaveDock(const char* path);
	 void Print();
} // namespace ImGui