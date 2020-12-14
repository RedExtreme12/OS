#pragma once

#include <Windows.h>
#include <string>
#include "WinException.h"

class Semaphore
{
public:
    Semaphore(LONG lInitialCount, LONG lMaximumCount, std::string name);
    Semaphore(DWORD dwDesiredAccess, BOOL bInheritHandle, std::string lpName);
    HANDLE get_handle_value();
    void signal();
    void wait();
    ~Semaphore();

private:
    HANDLE hSemaphore;
};