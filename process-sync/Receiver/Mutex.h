#pragma once

#include <Windows.h>
#include <string>
#include "WinException.h"

class Mutex
{
public:
    Mutex(std::string name);
    Mutex(DWORD dwDesiredAccess, BOOL bInitialOwner, std::string lpName);
    HANDLE get_handle_value();
    void acquire();
    void release();
    ~Mutex();
private:
    HANDLE hMutex;
};

