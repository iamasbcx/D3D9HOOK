#pragma once
#include <Windows.h>


//��麯���ķ���ֵ
void check_error(bool state,const char* str = nullptr)
{
	if (state) return;

	WCHAR buffer[1024 * 2];
	wsprintf(buffer, L"�������� : %s", str);
	MessageBoxW(nullptr, buffer, nullptr, MB_OK | MB_ICONHAND);
	
}

