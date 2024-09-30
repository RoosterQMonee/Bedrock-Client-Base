#pragma once

#include <cmath>


class Math
{
public:
	inline static float animate(float endPoint, float current, float speed) {
		if (speed < 0.0) speed = 0.0;
		else if (speed > 1.0) speed = 1.0;

		float dif = std::fmax(endPoint, current) - std::fmin(endPoint, current);
		float factor = dif * speed;

		return current + (endPoint > current ? factor : -factor);
	}

	inline static float lerp(float a, float b, float c) {
		return a + c * (b - a);
	}

	static inline float clamp(float value, float min_val, float max_val) {
		return value < min_val ? min_val : (value > max_val ? max_val : value);
	}
};