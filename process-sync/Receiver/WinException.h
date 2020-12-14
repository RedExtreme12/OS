#pragma once
#include <iostream>

class WinException :public std::exception
{
public:
	WinException(const char* msg, int error_code);
	int get_error_code();
private:
	int error_code;
};

