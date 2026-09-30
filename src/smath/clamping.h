#ifndef CLAMPING
#pragma once

#include "vectors.h"
#include "ivectors.h"
#include <format>

namespace smath
{
	// Minimum
	template<typename T>
	T min(T a, T b) { return (a <= b ? a : b); }
	template<typename T>
	T min(T a, T b, T c) { float result = (a <= b ? a : b); return (c <= result ? c : result); }
	inline vec2 min(vec2 a, vec2 b) { return vec2(min(a.x, b.x), min(a.y, b.y)); }
	inline vec3 min(vec3 a, vec3 b) { return vec3(min(a.x, b.x), min(a.y, b.y), min(a.z, b.z)); }
	inline ivec2 min(ivec2 a, ivec2 b) { return ivec2(min(a.x, b.x), min(a.y, b.y)); }


	// Maximum
	template<typename T>
	T max(T a, T b) { return (a >= b ? a : b); }
	template<typename T>
	T max(T a, T b, T c) { float result = (a >= b ? a : b); return (c >= result ? c : result); }

	// Clamping
	template<typename T>
	T clamp(T value, T valueMin, T valueMax) { return max(min(value, valueMax), valueMin); }
	inline vec2 clamp(vec2 value, float min, float max) { return vec2(clamp(value.x, min, max), clamp(value.y, min, max)); }
	inline float clamp01(float value) { return max(min(value, 1.0f), 0.0f); }
	inline int clamp01(int value) { return max(min(value, 1), 0); }

	// Lerping
	inline float lerp(float a, float b, float t) { return a + t * (b - a); }

	// Rounding
	inline std::string round(float value, int decimalPlaces)
	{
		return std::format("{:.{}f}", value, decimalPlaces);
	}
}

#endif // !CLAMPING
