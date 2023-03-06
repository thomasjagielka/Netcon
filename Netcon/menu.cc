#include <Windows.h>
#include <cstdio>
#include <stdio.h>
#include <thread>
#include <string>

#include "menu.h"
#include "settings.h"


const int kNumOfOptions = 2;

void menu::DisplayError(std::string text) {
	MessageBoxA(
		NULL,
		text.c_str(),
		"Netcon",
		MB_ICONERROR
	);
}

void menu::SetConsoleCursorVisibility(HANDLE console_handle, bool visible)
{
	CONSOLE_CURSOR_INFO cursorInfo;
	GetConsoleCursorInfo(console_handle, &cursorInfo);
	cursorInfo.bVisible = visible;
	SetConsoleCursorInfo(console_handle, &cursorInfo);
}

void SetCoordinates(HANDLE console_handle, int column, int line)
{
	COORD coordinates;
	coordinates.X = column;
	coordinates.Y = line;

	if (!SetConsoleCursorPosition(console_handle, coordinates))
		printf("SetCoordinates failed with an error: %d\n", GetLastError());
}

void menu::SetColor(HANDLE console_handle, int color) {
	SetConsoleTextAttribute(console_handle, color);
}

void PrintInColor(HANDLE console_handle, std::string text, int color = 7) {
	menu::SetColor(console_handle, color);
	printf(text.c_str());
	// Set color back to normal.
	//if (color != 7)
	//	SetColor(console_handle, 7);
}

void PrintWelcome(HANDLE console_handle) {
	SetCoordinates(console_handle, 0, 0);
	PrintInColor(console_handle, "Netcon", 11);
	PrintInColor(console_handle, " for ");
	PrintInColor(console_handle, "Counter Strike: Global Offensive", 11);
	PrintInColor(console_handle, " by ");
	PrintInColor(console_handle, "Thomas Jagielka", 11);
	PrintInColor(console_handle, ".\nUse arrows to control the menu options.\n\n");
}

void PrintBool(HANDLE console_handle, std::string text, bool is_enabled, int color = 7) {
	const size_t kTextLength = text.length();
	const int kStatusLength = is_enabled ? 2 : 3;
	const size_t kBlankLength = 37 - kTextLength - kStatusLength;

	std::string blank(kBlankLength, ' ');
	std::string kStatusText = is_enabled ? "ON" : "OFF";

	PrintInColor(console_handle, text + blank + "[" + kStatusText + "]\n", color);
}

//void menu::PrintError(HANDLE console_handle, std::string text, int line_index) {
//	SetCoordinates(console_handle, 0, line_index);
//	PrintInColor(console_handle, text + " Closing...", 12);
//}

int StartThread(HANDLE console_window, HANDLE console_handle) {
	int y = 3;
	bool line_changed = true;
	bool status_changed = false;

	do {
		Sleep(1);

		if (menu::terminate) {
			menu::SetConsoleCursorVisibility(console_handle, true);
			return 0;
		}

		const HWND kWindowActive = GetForegroundWindow();
		if (kWindowActive != console_window)
			continue;

		if (GetAsyncKeyState(VK_UP) & 1) {
			y--;
			line_changed = true;
		}
		if (GetAsyncKeyState(VK_DOWN) & 1) {
			y++;
			line_changed = true;
		}
		if (GetAsyncKeyState(VK_LEFT) & 1) {
			status_changed = true;
		}
		if (GetAsyncKeyState(VK_RIGHT) & 1) {
			status_changed = true;
		}

		if (line_changed || status_changed) {
			// Clamp.
			if (y < 3) y = 4;
			if (y > 4) y = 3;

			SetCoordinates(console_handle, 0, y);
		}

		if (line_changed) {
			if (y == kNumOfOptions + 1)
				PrintBool(console_handle, "Auto Damage Message", settings::damage_auto_message_enabled, 23);
			else {
				SetCoordinates(console_handle, 0, kNumOfOptions + 1);
				PrintBool(console_handle, "Auto Damage Message", settings::damage_auto_message_enabled);
			}

			if (y == kNumOfOptions + 2)
				PrintBool(console_handle, "Grenade Helper", settings::helper_grenade_enabled, 23);
			else {
				SetCoordinates(console_handle, 0, kNumOfOptions + 2);
				PrintBool(console_handle, "Grenade Helper", settings::helper_grenade_enabled);
			}

			line_changed = false;
		}

		if (status_changed) {
			if (y == kNumOfOptions + 1) {
				settings::damage_auto_message_enabled = !settings::damage_auto_message_enabled;
				PrintBool(console_handle, "Auto Damage Message", settings::damage_auto_message_enabled, 23);
			}

			if (y == kNumOfOptions + 2) {
				settings::helper_grenade_enabled = !settings::helper_grenade_enabled;
				PrintBool(console_handle, "Grenade Helper", settings::helper_grenade_enabled, 23);
			}

			status_changed = false;
		}
	} while (true);
}

void ClearMenu(HANDLE console_handle) {
	COORD top_left = { 0, 0 };
	CONSOLE_SCREEN_BUFFER_INFO screen;
	DWORD chars_written;

	GetConsoleScreenBufferInfo(console_handle, &screen);
	FillConsoleOutputCharacterA(
		console_handle, ' ', screen.dwSize.X * screen.dwSize.Y, top_left, &chars_written
	);
	FillConsoleOutputAttribute(
		console_handle, FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_BLUE,
		screen.dwSize.X * screen.dwSize.Y, top_left, &chars_written
	);
}

void menu::Init() {
	HANDLE console_window = GetConsoleWindow();
	menu::console_handle = GetStdHandle(STD_OUTPUT_HANDLE);

	ClearMenu(console_handle);
	PrintWelcome(console_handle);
	SetConsoleCursorVisibility(console_handle, false);
	StartThread(console_window, console_handle);
}