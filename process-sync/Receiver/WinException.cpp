#include "WinException.h"


WinException::WinException(const char* msg, int error_code):exception(msg)
{
	this->error_code = error_code;
}

int WinException::get_error_code()
{
	return this->error_code;
}
