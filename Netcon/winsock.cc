#define WIN32_LEAN_AND_MEAN

#include <Windows.h>
#include <ws2tcpip.h>
#include <string>

#include "winsock.h"
#include "menu.h"

// Need to link with Ws2_32.lib.
#pragma comment (lib, "Ws2_32.lib")


const PCSTR kPort = "2121";

int GetMyAddrInfo(PCSTR kIp, PCSTR kPort, struct addrinfo ai_hints, struct addrinfo *&ai_result) {
    // Resolve the server address and port
    const int kResult = getaddrinfo(kIp, kPort, &ai_hints, &ai_result);
    if (kResult != 0) {
        menu::DisplayError("getaddrinfo failed with an error: " + std::to_string(kResult) + ".");
        //WSACleanup();
        return 1;
    }

    return 0;
}

int ConnectSocket(struct addrinfo *ai_ptr, struct addrinfo *ai_result, SOCKET &connect_socket) {
    // Attempt to connect to an address until one succeeds.
    for (ai_ptr = ai_result; ai_ptr != nullptr; ai_ptr = ai_ptr->ai_next) {

        // Create a SOCKET for connecting to server.
        connect_socket = socket(ai_ptr->ai_family, ai_ptr->ai_socktype,
            ai_ptr->ai_protocol);
        if (connect_socket == INVALID_SOCKET) {
            menu::DisplayError("socket failed with an error: " + std::to_string(WSAGetLastError()) + ".");
            //WSACleanup();
            return 1;
        }

        // Connect to the server.
        const int kResult = connect(connect_socket, ai_ptr->ai_addr, (int)ai_ptr->ai_addrlen);
        if (kResult == SOCKET_ERROR) {
            closesocket(connect_socket);
            connect_socket = INVALID_SOCKET;
            continue;
        }

        break;
    }

    freeaddrinfo(ai_result);

    if (connect_socket == INVALID_SOCKET) {
        menu::DisplayError(std::string("Unable to connect to the server!"));
        //WSACleanup();
        return 1;
    }

    return 0;
}

int winsock::SendToConsole(SOCKET connect_socket, const char *send_buf) {
    const int kResult = send(connect_socket, send_buf, (int)strlen(send_buf), 0);
    if (kResult == SOCKET_ERROR) {
        //menu::PrintError(menu::console_handle,
        //    std::string("send failed with error: %d" + std::to_string(WSAGetLastError()) + "."),
        //    error_line_no);
        menu::DisplayError(std::string("send failed with an error: " + std::to_string(WSAGetLastError()) + "."));
        return SOCKET_ERROR;
    }

    return kResult;
}

//int winsock::Cleanup(SOCKET connect_socket, int return_value) {
//    closesocket(connect_socket);
//    WSACleanup();
//    return return_value;
//}

int winsock::Init(PCSTR kIp, PCSTR kPort) {
    WSADATA wsa_data;
    struct addrinfo ai_hints;

    const int kResult = WSAStartup(MAKEWORD(2, 2), &wsa_data);
    if (kResult != 0) {
        menu::DisplayError("WSAStartup failed with an error: " + std::to_string(kResult) + ".");
        return 1;
    }

    ZeroMemory(&ai_hints, sizeof(ai_hints));
    ai_hints.ai_family = AF_UNSPEC;
    ai_hints.ai_socktype = SOCK_STREAM;
    ai_hints.ai_protocol = IPPROTO_TCP;

    struct addrinfo *ai_result = nullptr;
    if (GetMyAddrInfo(kIp, kPort, ai_hints, ai_result))
        return 1;

    struct addrinfo *ai_ptr = nullptr;
    if (ConnectSocket(ai_ptr, ai_result, winsock::connect_socket))
        return 1;

    if (winsock::SendToConsole(winsock::connect_socket, "echo Netcon initialized.\r\n") == -1)
        return 1;

    return 0;
}