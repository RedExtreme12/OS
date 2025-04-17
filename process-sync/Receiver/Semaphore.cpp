#include "Semaphore.h"

Semaphore::Semaphore(LONG lInitialCount, LONG lMaximumCount, std::string name)
{
    this->hSemaphore = CreateSemaphore(NULL, lInitialCount, lMaximumCount, (LPCSTR)(name.c_str()));

    if (!this->hSemaphore)
    {
        throw WinException("Create Semaphore failed", GetLastError());
    }
}

Semaphore::Semaphore(DWORD dwDesiredAccess, BOOL bInheritHandle, std::string lpName)
{
    this->hSemaphore = OpenSemaphore(dwDesiredAccess, bInheritHandle, (LPCSTR)(lpName.c_str()));

    if (!this->hSemaphore)
    {
        throw WinException("Open Semaphore failed", GetLastError());
    }
}

HANDLE Semaphore::get_handle_value()
{
    if (this->hSemaphore)
        return this->hSemaphore;

    return INVALID_HANDLE_VALUE;
}

void Semaphore::signal()
{
    bool result = ReleaseSemaphore(this->hSemaphore, 1, NULL);

    if (!result)
    {
        throw WinException("Signaling failed", GetLastError());
    }
}

void Semaphore::wait()
{
    if (WaitForSingleObject(this->hSemaphore, INFINITE) == WAIT_FAILED)
    {
        throw WinException("Waiting failed", GetLastError());
    }
}

Semaphore::~Semaphore()
{
    CloseHandle(this->hSemaphore);
}