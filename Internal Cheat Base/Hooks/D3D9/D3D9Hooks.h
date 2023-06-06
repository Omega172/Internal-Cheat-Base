#pragma once
#include <D3D9.h>
#include <DXGI.h>

#include "../../Globals/Globals.h"
#include "../../Libraries/Kiero/kiero.h"

#include "../../Libraries/ImGUI/imgui.h"
#include "../../Libraries/ImGUI/imgui_impl_win32.h"
#include "../../Libraries/ImGUI/imgui_impl_dx9.h"

#include "../../Libraries/ImGUI/Styles.h"
#include "../../GUI/GUI.h"

namespace D3D9
{
	typedef HRESULT(__stdcall* EndScene)(LPDIRECT3DDEVICE9);
	typedef HRESULT(__stdcall* Reset)(LPDIRECT3DDEVICE9, D3DPRESENT_PARAMETERS*);

	EndScene oEndScene;
	Reset oReset;
	WNDPROC oWndProc;

	HWND window = NULL;

	bool hkInit = false;

	void InitImGui(LPDIRECT3DDEVICE9 pDevice)
	{
		D3DDEVICE_CREATION_PARAMETERS params;
		pDevice->GetCreationParameters(&params);

		window = params.hFocusWindow;

		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO();
		io.ConfigFlags = ImGuiConfigFlags_NoMouseCursorChange;
		ImGui_ImplWin32_Init(window);
		ImGui_ImplDX9_Init(pDevice);

		SetupStyle();
		ImportFonts();
	}

	LRESULT __stdcall WndProc(const HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		if (GUI::bMenuOpen && ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam))
			return true;

		return (GUI::bMenuOpen) ? GUI::bMenuOpen : CallWindowProc(oWndProc, hWnd, uMsg, wParam, lParam);
	}

	HRESULT __stdcall hkEndScene(LPDIRECT3DDEVICE9 pDevice)
	{
		if (!hkInit)
		{
			InitImGui(pDevice);
			hkInit = true;
		}

		// Fix washedout colors (uncomment if needed)
		//DWORD colorwrite, srgbwrite;
		//{
		//	pDevice->GetRenderState(D3DRS_COLORWRITEENABLE, &colorwrite);
		//	pDevice->GetRenderState(D3DRS_SRGBWRITEENABLE, &srgbwrite);

		//	//fix drawing without calling engine functons/cl_showpos
		//	pDevice->SetRenderState(D3DRS_COLORWRITEENABLE, 0xffffffff);
		//	//removes the source engine color correction
		//	pDevice->SetRenderState(D3DRS_SRGBWRITEENABLE, false);
		//}

		GUI::BeginRender();
		ImGui::PushFont(tahomaFont);
		GUI::Render();
		ImGui::PopFont();
		GUI::EndRender();

		ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());

		// End fix washedout colors (uncomment if needed)
		//{
		//	pDevice->SetRenderState(D3DRS_COLORWRITEENABLE, colorwrite);
		//	pDevice->SetRenderState(D3DRS_SRGBWRITEENABLE, srgbwrite);
		//}

		if (!cGlobals.get()->IsInitalized())
		{
			pDevice->Release();
			oWndProc = (WNDPROC)SetWindowLongPtr(window, GWLP_WNDPROC, (LONG_PTR)(oWndProc));
			oEndScene(pDevice);
			kiero::shutdown();
			return 0;
		}

		return oEndScene(pDevice);
	}

	HRESULT __stdcall hkReset(LPDIRECT3DDEVICE9 pDevice, D3DPRESENT_PARAMETERS* pPresentationParameters)
	{
		ImGui_ImplDX9_InvalidateDeviceObjects();
		long result = oReset(pDevice, pPresentationParameters);
		ImGui_ImplDX9_CreateDeviceObjects();

		return result;
	}
}