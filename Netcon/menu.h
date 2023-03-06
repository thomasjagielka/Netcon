#pragma once

#include <Windows.h>


namespace menu {
	void DisplayError(std::string text);
	//void PrintError(HANDLE console_handle, std::string text, int line_index = 0);
	inline HANDLE console_handle;
	void SetConsoleCursorVisibility(HANDLE console_handle, bool visible);
	void SetColor(HANDLE console_handle, int color);
	inline bool terminate;
	void Init();
}