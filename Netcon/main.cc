#define WIN32_LEAN_AND_MEAN

#include <Windows.h>
#include <winsock2.h>
#include <stdio.h>
#include <string>
#include <thread>

#include "menu.h"
#include "settings.h"
#include "features.h"
#include "winsock.h"


const PCSTR kDefaultIp = "127.0.0.1";
const PCSTR kDefaultPort = "2121";

int sendThread() {
    do {
        if (winsock::SendToConsole(winsock::connect_socket, "getpos\r\n") == -1) {
            menu::terminate = true;
            return 1;
        }
        Sleep(50);
    } while (true);

    return 0;
}

void Exit() {
    if (winsock::connect_socket != INVALID_SOCKET)
        if (closesocket(winsock::connect_socket))
            menu::DisplayError("closesocket failed with an error: " + std::to_string(WSAGetLastError()) + ".");
    if (WSACleanup())
        menu::DisplayError("WSACleanup failed with an error: " + std::to_string(WSAGetLastError()) + ".");

    menu::SetConsoleCursorVisibility(menu::console_handle, true);
    // Set color back to normal.
    menu::SetColor(menu::console_handle, 7);
}

// Handle CTRL+C shortcut.
BOOL CtrlHandler(DWORD fdwCtrlType) {
    Exit();
    return 0;
}

int main(int argc, char* argv[])
{
    PCSTR ip = kDefaultIp;
    PCSTR port = kDefaultPort;

    if (argv[1])
        ip = argv[1];
    if (argc > 2)
        if (argv[2])
            port = argv[2];

    if (winsock::Init(ip, port)) {
        // Exit function not yet initialized, so call it now.
        Exit();
        return 1;
    }

    // Initialize the Exit function.
    atexit(Exit);
    if (!SetConsoleCtrlHandler(CtrlHandler, true)) {
        menu::DisplayError("SetConsoleCtrlHandler failed with an error: " + std::to_string(GetLastError()) + ".");
        return 1;
    }

    std::thread kMenuThread(menu::Init);

    const std::thread kSendThread(sendThread);

    // Receive until the peer closes the connection.
    // Code here executes only when we receive data.
    int result = 1;
    do {
        if (!settings::damage_auto_message_enabled && !settings::helper_grenade_enabled) {
            Sleep(1);
            continue;
        }
        
        // Read character by character the game console output.
        char recv_buf;
        int result = recv(winsock::connect_socket, &recv_buf, 1, 0);
        if (result > 0) {
            // Save read line for later use.
            static std::string kLine;
            kLine += recv_buf;

            if (recv_buf == '\n') {
                // Call our features.

                if (settings::damage_auto_message_enabled)
                    if (features::AutoDamageMessage(kLine)) {
                        menu::terminate = true;
                        kMenuThread.join();
                        return 1;
                    }

                if (settings::helper_grenade_enabled)
                    if (features::GrenadeHelper(kLine)) {
                        menu::terminate = true;
                        kMenuThread.join();
                        return 1;
                    }

                // Ready to read the next line.
                kLine.clear();
            }
        }
        else if (result == 0) {
            menu::DisplayError("Connection closed.");
            menu::terminate = true;
            kMenuThread.join();
            return 1;
        }
        else {
            menu::DisplayError("recv failed with an error: " + std::to_string(WSAGetLastError()) + ".");
            menu::terminate = true;
            kMenuThread.join();
            return 1;
        }

    } while (result > 0);

    menu::terminate = true;
    kMenuThread.join();
    return 0;
}