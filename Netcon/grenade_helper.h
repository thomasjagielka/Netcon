#pragma once

#include <string>

#include "math.h"

struct Grenade {
	int tickrate;
	std::string map_name;
	bool jumpthrow;
	std::string grenade_type;
	std::string grenade_description;
	XyzPoint position;
	XyPoint angle;

	std::string to_string() {
		return std::to_string(tickrate) +
			" | " + map_name +
			" | " + std::to_string(jumpthrow) +
			" | " + grenade_type +
			" | " + grenade_description +
			" | " + position.to_string() +
			" | " + angle.to_string();
	}
};