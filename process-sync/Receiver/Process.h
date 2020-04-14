#include <Windows.h>
#include <string>
#include "WinException.h"

#pragma once
class Process
{
public:
    Process(std::string lpCommandLineCreator);
    DWORD work();
    HANDLE get_handle_value();
    ~Process();

private:
    HANDLE hProcess;
    STARTUPINFO siCreator;
    PROCESS_INFORMATION piCreator;
    std::string lpCommandLine;
};