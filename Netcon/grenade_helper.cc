#include <string>
#include <vector>
#include <string>
#include <algorithm>

#include "features.h"
#include "winsock.h"
#include "math.h"
#include "grenade_helper.h"


int features::GrenadeHelper(std::string line) {
    if (line.rfind("setpos", 0) == 0) {
        std::replace(line.begin(), line.end(), ';', ' ');

        size_t search_position = 0;
        std::string splitted_text;
        int splitted_text_index = 0;
        XyzPoint position;
        XyPoint angle;

        Grenade sample_grenade;
        sample_grenade.tickrate = 64;
        sample_grenade.map_name = "Mirage";
        sample_grenade.jumpthrow = false;
        sample_grenade.grenade_type = "Smoke";
        sample_grenade.grenade_description = "From trash can to Middle";
        sample_grenade.position = { 1423, 71, -49 };
        sample_grenade.angle = { /*-38*/0, /*-161*/170};


        while ((search_position = line.find(" ")) != std::string::npos) {
            splitted_text = line.substr(0, search_position);

            // Skip setpos and setang strings.
            switch (splitted_text_index) {
                case 1:
                    position.x = std::stof(splitted_text);
                    break;
                case 2:
                    position.y = std::stof(splitted_text);
                    break;
                case 3:
                    position.z = std::stof(splitted_text);
                    break;

                case 5:
                    angle.x = /*std::stof(splitted_text)*/0;
                    break;
                //case 6:
                //    angle.y = std::stof(splitted_text);
                //    break;

            }

            if (splitted_text_index == 6) {
                float y = std::stof(splitted_text);
                if (abs(y - sample_grenade.angle.y) < 180)
                    angle.y = abs(y - sample_grenade.angle.y);
                else
                    angle.y = 360 - abs(y - sample_grenade.angle.y);
            }

            line.erase(0, search_position + 1);
            splitted_text_index++;
        }

        /*winsock::SendToConsole(winsock::connect_socket, ("echo " + position.to_string() + "  " + angle.to_string() + "\n\r").c_str());*/
        //float position_distance = position.distance({ 0, 0, 0 });
        //winsock::SendToConsole(winsock::connect_socket, ("echo " + std::to_string(position_distance) + "\n\r").c_str());

        //float angle_distance = angle.distance({ 0, 0 });
        //winsock::SendToConsole(winsock::connect_socket, ("echo " + std::to_string(angle_distance) + "\n\r").c_str());

        winsock::SendToConsole(winsock::connect_socket, ("echo " + sample_grenade.to_string() + "\n\r").c_str());

        winsock::SendToConsole(winsock::connect_socket, ("echo " +
            std::to_string(position.distance(sample_grenade.position)) +
            " | " + std::to_string(angle.distance(sample_grenade.angle)) + 
            "\n\r").c_str());
    }

	return 0;
}