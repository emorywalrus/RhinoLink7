#pragma once
#include <cmath>

class vec3 {
	long d[3] = {0,0,0};
	vec3(long dir[3]) {
		d[0] = dir[0];
		d[1] = dir[1];
		d[2] = dir[2];
	}
	long dot(vec3 other) {
		return (d[0] * other.d[0]) + (d[1] * other.d[1]) + (d[2] * other.d[2]);
	}
	void cross(vec3 other) {
		long x = d[1] * other.d[2] - d[2] * other.d[1];
		long y = d[0] * other.d[2] - d[2] * other.d[0];
		long z = d[0] * other.d[1] - d[1] * other.d[0];
		d[0] = x;
		d[1] = y;
		d[2] = x;
	}
	void add(vec3 other) {
		d[0] = d[0] + other.d[0];
		d[1] = d[1] + other.d[1];
		d[2] = d[2] + other.d[2];
	}
	void subtract(vec3 other) {
		d[0] = d[0] - other.d[0];
		d[1] = d[1] - other.d[1];
		d[2] = d[2] - other.d[2];
	}
	void multiply(long scalar) {
		d[0] = d[0] * scalar;
		d[1] = d[1] * scalar;
		d[2] = d[2] * scalar;
	}
	void divide(long scalar) {
		d[0] = d[0] / scalar;
		d[1] = d[1] / scalar;
		d[2] = d[2] / scalar;
	}
	long length() {
		return (long) std::sqrt(d[0] * d[0] + d[1] * d[1] + d[2] * d[2]);
	}
	void set_length(long new_length) {
		long numerator = (new_length * 100000) / this->length();
		this->multiply(numerator);
		this->divide(1000);
	}
};
