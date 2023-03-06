#pragma once

#include <Windows.h>


namespace winsock {
	inline SOCKET connect_socket = INVALID_SOCKET;
	int SendToConsole(SOCKET connect_socket, const char *send_buf);
	//int Cleanup(SOCKET connect_socket, int return_value = 0);
	int Init(PCSTR kIp, PCSTR kPort);
}