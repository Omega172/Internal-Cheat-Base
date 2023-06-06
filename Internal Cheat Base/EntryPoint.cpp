#include <Windows.h>
#include <vector>
#include <thread>

// Globals
#include "Globals/Globals.h"
extern Globals* pGlobals = cGlobals.get();

// Utilities
#include "Utils/Logging/Logging.hpp"

// Libraries
#include "Libraries/Kiero/kiero.h"

// Graphics APIS
#include "Hooks/D3D9/D3D9Hooks.h"
#include "Hooks/D3D11/D3D11Hooks.h"

DWORD WINAPI MainThread(LPVOID lpParam)
{
	if (pGlobals->Con->IsAllocated())
		Log("Init", "Console allocated\n");
	else
		pGlobals->Shutdown();

	kiero::RenderType::Enum renderType = kiero::RenderType::D3D9;

	if (kiero::init(renderType) == kiero::Status::Success)
	{
		switch (kiero::getRenderType())
		{
			// Make sure to change the defines in the to of kiero.h if you plan on using a different graphics api

		case kiero::RenderType::D3D9:
			if (kiero::bind(42, (void**)&D3D9::oEndScene, D3D9::hkEndScene) != kiero::Status::Success || kiero::bind(16, (void**)&D3D9::oReset, D3D9::hkReset) != kiero::Status::Success)
				pGlobals->Shutdown();
			break;

		case kiero::RenderType::D3D11:
			if (kiero::bind(8, (void**)&D3D11::oPresent, D3D11::hkPresent) != kiero::Status::Success)
				pGlobals->Shutdown();
			break;

		default:
			pGlobals->Shutdown();
		}
	}

	while (pGlobals->IsInitalized())
	{
		if (GetAsyncKeyState(pGlobals->_UnloadKey) & 0x1)
			break;

		if (GetAsyncKeyState(pGlobals->_MenuKey) & 0x1)
			GUI::bMenuOpen = !GUI::bMenuOpen;
	}

	Log("Shutdown", "Shutting down...\n");

	std::this_thread::sleep_for(std::chrono::seconds(3));
	FreeLibraryAndExitThread((HMODULE)lpParam, EXIT_SUCCESS);
	return TRUE;
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ulReasonForCall, LPVOID lpReserved)
{
	DisableThreadLibraryCalls(hModule);

	if (ulReasonForCall == DLL_PROCESS_ATTACH)
		CreateThread(nullptr, NULL, MainThread, hModule, NULL, nullptr);

	if (ulReasonForCall == DLL_PROCESS_DETACH)
		pGlobals->Shutdown();

	return TRUE;
}