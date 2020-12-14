#pragma once

#include "Process.h"

Process::Process(std::string _lpCommandLineCreator)
{
    this->lpCommandLine = _lpCommandLineCreator;

    ZeroMemory(&siCreator, sizeof(STARTUPINFO));
    siCreator.cb = sizeof(STARTUPINFO);
}

DWORD Process::work()
{
    bool result;

    result = CreateProcess(NULL, (LPSTR)((this->lpCommandLine).c_str()), NULL, NULL, FALSE,
        CREATE_NEW_CONSOLE, NULL, NULL, &this->siCreator, &this->piCreator);
    this->hProcess = piCreator.hProcess;

    if (!result)
    {
        throw WinException("Create Process failed", GetLastError());
    }        

    return 0;
}

HANDLE Process::get_handle_value()
{
    if (this->hProcess)
    {
        return this->hProcess;
    }

    return INVALID_HANDLE_VALUE;
}

Process::~Process()
{
    CloseHandle(this->piCreator.hThread);
    CloseHandle(this->piCreator.hProcess);
}