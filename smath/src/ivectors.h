#ifndef IVECTORS
#pragma once

#include <iostream>
#include <cmath>
#include "vectors.h"

namespace smath
{

	// * ------------------------ *
	// |  Integer Vector Structs  |
	// * ------------------------ *

	struct ivec2
	{
		// Constructors
		ivec2() : x(0), y(0) {}
		ivec2(const int& x, const int& y) : x(x), y(y) {}
		ivec2(const int& value) : x(value), y(value) {}

		ivec2(const ivec2& vec) : x(vec.x), y(vec.y) {} // Copy constructor

		// Operator Overloads w/ other vector
		ivec2 operator+(const ivec2& aVec) const;
		ivec2& operator+=(const ivec2& aVec);

		ivec2 operator-(const ivec2& aVec) const;
		ivec2& operator-=(const ivec2& aVec);

		ivec2 operator*(const ivec2& aVec) const;
		ivec2& operator*=(const ivec2& aVec);

		ivec2 operator/(const ivec2& aVec) const;
		ivec2& operator/=(const ivec2& aVec);

		ivec2& operator=(const ivec2& aVec);

		// Operator Overloads w/ a scalar
		ivec2 operator*(const int& scalar) const;
		ivec2& operator*=(const int& scalar);

		ivec2 operator/(const int& scalar) const;
		ivec2& operator/=(const int& scalar);

		// Boolean operators
		bool operator==(const ivec2& aVec) const;
		bool operator!=(const ivec2& aVec) const;

		// ostream Operator
		friend std::ostream& operator<<(std::ostream& os, const ivec2& vec);

		// Conversion operator
		operator vec2() const { return vec2((float)this->x, (float)this->y); }

		// Member Variables
		int x;
		int y;
	};
}

#endif // !IVECTORS