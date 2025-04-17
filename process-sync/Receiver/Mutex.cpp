#include "Mutex.h"


Mutex::Mutex(std::string name)
{
    this->hMutex = CreateMutex(NULL, FALSE, (LPCSTR)(name.c_str()));

    if (!this->hMutex)
    {
        throw WinException("Create Mutex failed", GetLastError());
    }
}

Mutex::Mutex(DWORD dwDesiredAccess, BOOL bInitialOwner, std::string lpName)
{
    this->hMutex = OpenMutex(dwDesiredAccess, bInitialOwner, (LPCSTR)(lpName.c_str()));

    if (!this->hMutex)
    {
        throw WinException("Open Mutex failed", GetLastError());
    }
}

HANDLE Mutex::get_handle_value()
{
    if (this->hMutex)
    {
        return this->hMutex;
    }

    return INVALID_HANDLE_VALUE;
}

void Mutex::acquire()
{
    if (WaitForSingleObject(this->hMutex, INFINITE) == WAIT_FAILED)
    {
        throw WinException("Acquire failed", GetLastError());
    }
}

void Mutex::release()
{
    bool result = ReleaseMutex(this->hMutex);

    if (!result)
    {
        throw WinException("Release Semaphore failed", GetLastError());
    }
}

Mutex::~Mutex()
{
    CloseHandle(this->hMutex);
}