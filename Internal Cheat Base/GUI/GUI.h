#pragma once
#include "../Globals/Globals.h"

#include "../Libraries/Kiero/kiero.h"
#define IMGUI_DEFINE_MATH_OPERATORS
#include "../Libraries/ImGUI/imgui.h"
#include "../Libraries/ImGUI/imgui_impl_win32.h"

#include "../Libraries/ImGUI/imgui_impl_dx9.h"
#include "../Libraries/ImGUI/imgui_impl_dx11.h"

extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

namespace GUI
{
	inline bool bMenuOpen = false;
	inline float WIDTH = 700;
	inline float HEIGHT = 400;

	inline void BeginRender()
	{
		switch (kiero::getRenderType())
		{
		case kiero::RenderType::D3D9:
			ImGui_ImplDX9_NewFrame();
			break;

		case kiero::RenderType::D3D11:
			ImGui_ImplDX11_NewFrame();
			break;

		default:
			cGlobals.get()->Shutdown();
			return;
		}

		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();

		ImGuiIO& io = ImGui::GetIO();
	}

	void Render();

	inline void EndRender()
	{
		switch (kiero::getRenderType())
		{
		case kiero::RenderType::D3D9:
			break;

		case kiero::RenderType::D3D11:
			break;

		default:
			cGlobals.get()->Shutdown();
			return;
		}

		ImGui::EndFrame();
		ImGui::Render();
	}
}