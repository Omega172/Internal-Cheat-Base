#include "Console.hpp"

bool Console::_Allocated = false;
Console* Console::_Singleton = nullptr;

FILE* Console::stdoutDummy = nullptr;
FILE* Console::stdinDummy = nullptr;

Console::Console(std::string Title, bool Visibility)
{
    if (!AllocConsole()) {
        std::cout << "AllocConsole() Error: " << GetLastError() << std::endl;
        return;
    }
    else
        _Allocated = true;

    freopen_s(&stdoutDummy, "CONOUT$", "w", stdout);
    freopen_s(&stdinDummy, "CONIN$", "r", stdin);
    std::cout.clear();
    std::cin.clear();

    SetTitle(Title);
    SetVisibility(Visibility);
}

Console* Console::Instance(std::string Title, bool Visibility)
{
    if (_Singleton == nullptr)
        _Singleton = new Console(Title, Visibility);

    return _Singleton;
}

void Console::Destroy()
{
    if (_Singleton == nullptr)
        return;

    if (stdoutDummy != nullptr)
        fclose(stdoutDummy);
    if (stdinDummy != nullptr)
        fclose(stdinDummy);

    if (!FreeConsole())
    {
        std::cout << "FreeConsole() Error: " << GetLastError() << std::endl;
        return;
    }

    _Allocated = false;
}

void Console::SetVisibility(bool Visibility)
{
    _Visibility = Visibility;
    ShowWindow(GetConsoleWindow(), (Visibility) ? SW_SHOW : SW_HIDE);
    return;
}

void Console::ToggleVisibility()
{
    SetVisibility(!_Visibility);
    return;
}