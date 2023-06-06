#pragma once
#include <Windows.h>
#include <iostream>

class Console
{
protected:
	static bool _Allocated;
	static Console* _Singleton;

	bool _Visibility = false;

	static FILE* stdoutDummy;
	static FILE* stdinDummy;

	Console(std::string Title, bool Visibility);

public:
	Console(Console& Other) = delete;
	void operator=(const Console&) = delete;

	static Console* Instance(const std::string Title, bool Visibility);
	static void Destroy();

	void SetVisibility(bool Visibility);
	void ToggleVisibility();
	bool GetVisibility() { return _Visibility; };

	void SetTitle(std::string title) { SetConsoleTitleA(title.c_str()); };

	bool IsAllocated() { return _Allocated; };
};