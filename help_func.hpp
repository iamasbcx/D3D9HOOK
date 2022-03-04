#pragma once
#include <Windows.h>


//检查函数的返回值
void check_error(bool state,const char* str = nullptr)
{
	if (state) return;

	WCHAR buffer[1024 * 2];
	wsprintf(buffer, L"发生错误 : %s", str);
	MessageBoxW(nullptr, buffer, nullptr, MB_OK | MB_ICONHAND);
	
}

