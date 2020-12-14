#include "Event.h"


Event::Event(BOOL bManualReset, BOOL bInitialState, std::string lpName)
{
    this->hEvent = CreateEvent(NULL, bManualReset, bInitialState, (LPCSTR)(lpName.c_str()));

    if (!this->hEvent)
    {
        throw WinException("Create Event failed", GetLastError());
    }
}

Event::Event(DWORD dwDesiredAccess, BOOL bInheritHandle, std::string lpName)
{
    this->hEvent = OpenEvent(dwDesiredAccess, bInheritHandle, (LPCSTR)(lpName.c_str()));

    if (!this->hEvent)
    {
        throw WinException("Open Event failed", GetLastError());
    }
}

HANDLE Event::get_event_handle()
{
    if (this->hEvent)
    {
        return this->hEvent;
    }

    return INVALID_HANDLE_VALUE;;
}

void Event::set_event()
{
    bool result = SetEvent(this->hEvent);

    if (!result)
    {
        throw WinException("Set Event failed", GetLastError());
    }
}

Event::~Event()
{
    CloseHandle(this->hEvent);
}