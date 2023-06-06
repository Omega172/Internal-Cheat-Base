#pragma once
#include "../Utils/Console/Console.hpp"

class Globals
{
private:
	bool Initalized;

public:
	Globals(DWORD MenuKey = VK_INSERT, DWORD UnloadKey = VK_END) : _MenuKey(MenuKey), _UnloadKey(UnloadKey)
	{
		Con = Console::Instance("Cheat Base", true);

		// Do Setup

		Initalized = true;
	}

	bool IsInitalized() { return Initalized; };
	void Shutdown()
	{ 
		if (Con != nullptr)
			Con->Destroy();

		Initalized = false;
	};

	Console* Con = nullptr;

	DWORD _MenuKey = 0x0;
	DWORD _UnloadKey = 0x0;
};

inline std::unique_ptr<Globals> cGlobals = std::make_unique<Globals>();