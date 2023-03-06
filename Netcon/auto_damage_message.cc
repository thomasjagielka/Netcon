#include <Windows.h>
#include <string>

#include "settings.h"
#include "features.h"
#include "winsock.h"


int features::AutoDamageMessage(std::string line) {
    if (line.rfind("Damage Given", 0) == 0) {
        std::string start_delimiter = "- ";
        std::string last_delimiter = " i";
        size_t first = line.find(start_delimiter) + start_delimiter.length();
        size_t last = line.find(last_delimiter) - first;
        std::string damage = line.substr(first, last);

        if (std::stoi(damage) >= 100)
            return 0;

        std::string message = "say_team ";
        message += line;

        if (winsock::SendToConsole(winsock::connect_socket, message.c_str()) == -1)
            return 1;

        // Minimum (?) delay between chat messages.
        // Replace with curtime (?).
        Sleep(675);
    }

    //if (line.find("rel=1") != std::string::npos && (line.find("wire=116") != std::string::npos || line.find("wire=124") != std::string::npos || line.find("wire=81") != std::string::npos || line.find("wire=77") != std::string::npos || line.find("wire=87") != std::string::npos || line.find("wire=89") != std::string::npos)) {
    //    if (winsock::SendToConsole(winsock::connect_socket, "echo \"PLAYER NEARBY\"\n\r") == -1)
    //        return 1;
    //}

    return 0;
}