#pragma once

#include <string>


struct XyPoint {
	float x, y;

	std::string to_string() {
		return std::to_string(x) + " " + std::to_string(y);
	}

	XyPoint operator-(XyPoint xy_point) {
		return { x - xy_point.x, y - xy_point.y };
	}

	XyPoint raise_to_pow(int exponent) {
		return { powf(x, exponent), powf(y, exponent) };
	}

	float sum() {
		return x + y;
	}

	float distance(XyPoint xy_point) {
		XyPoint divided = *this - xy_point;
		XyPoint raised = divided.raise_to_pow(2);
		float sum = raised.sum();
		return sqrt(sum);
	}
};

struct XyzPoint {
	float x, y, z;

	std::string to_string() {
		return std::to_string(x) + " " + std::to_string(y) + " " + std::to_string(z);
	}

	XyzPoint operator-(XyzPoint xyz_point) {
		return { x - xyz_point.x, y - xyz_point.y, z - xyz_point.z };
	}

	XyzPoint raise_to_pow(int exponent) {
		return { powf(x, exponent), powf(y, exponent), powf(z, exponent) };
	}

	float sum() {
		return x + y + z;
	}

	float distance(XyzPoint xyz_point) {
		XyzPoint divided = *this - xyz_point;
		XyzPoint raised = divided.raise_to_pow(2);
		float sum = raised.sum();
		return sqrt(sum);
	}
};