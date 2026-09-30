#ifndef VECTORS
#pragma once

#include <SDL3/SDL.h>
#include <iostream>
#include <cmath>

namespace smath
{

	struct ivec2;

	// * ---------------- *
	// |  Vector Structs  |
	// * ---------------- *

	struct vec2
	{
		// Constructors
		vec2() : x(0), y(0) {}
		vec2(const int& x, const int& y) : x(x), y(y) {}
		vec2(const float& x, const float& y) : x(x), y(y) {}
		vec2(const float& value) : x(value), y(value) {}
		vec2(const int& value) : x(value), y(value) {}

		vec2(const vec2& vec) : x(vec.x), y(vec.y) {} // Copy constructor

		// Operator Overloads w/ other vector
		vec2 operator+(const vec2& aVec) const;
		vec2& operator+=(const vec2& aVec);

		vec2 operator-(const vec2& aVec) const;
		vec2& operator-=(const vec2& aVec);

		vec2 operator*(const vec2& aVec) const;
		vec2& operator*=(const vec2& aVec);

		vec2 operator/(const vec2& aVec) const;
		vec2& operator/=(const vec2& aVec);

		vec2& operator=(const vec2& aVec);

		// Operator Overloads w/ a scalar
		vec2 operator*(const float& scalar) const;
		vec2& operator*=(const float& scalar);

		vec2 operator/(const float& scalar) const;
		vec2& operator/=(const float& scalar);

		// Boolean operators
		bool operator==(const vec2& aVec) const;
		bool operator!=(const vec2& aVec) const;
		bool operator>=(const vec2& aVec) const; // Compares lengths
		bool operator>(const vec2& aVec) const;  // Compares lengths
		bool operator<=(const vec2& aVec) const; // Compares lengths
		bool operator<(const vec2& aVec) const;  // Compares lengths

		// ostream Operator
		friend std::ostream& operator<<(std::ostream& os, const vec2& vec);

		// Conversion operators
		explicit operator ivec2() const;
		operator SDL_FPoint() const { return { x, y }; }

		// Length Calculation
		float length() const;

		// Member Variables
		float x;
		float y;
	};

	struct vec3
	{
		// Constructors
		vec3() : x(0), y(0), z(0) {}
		vec3(const int& x, const int& y, const int& z) : x(x), y(y), z(z) {}
		vec3(const float& x, const float& y, const float& z) : x(x), y(y), z(z) {}
		vec3(const float& value) : x(value), y(value), z(value) {}
		vec3(const int& value) : x(value), y(value), z(value) {}

		vec3(const vec2& vec, const float& z = 0.0f) : x(vec.x), y(vec.y), z(z) {}
		vec3(const vec3& vec) : x(vec.x), y(vec.y), z(vec.z) {} // Copy Constructor

		// Operator Overloads w/ other vector
		vec3 operator+(const vec3& aVec) const;
		vec3& operator+=(const vec3& aVec);

		vec3 operator-(const vec3& aVec) const;
		vec3& operator-=(const vec3& aVec);

		vec3 operator*(const vec3& aVec) const;
		vec3& operator*=(const vec3& aVec);

		vec3 operator/(const vec3& aVec) const;
		vec3& operator/=(const vec3& aVec);

		vec3& operator=(const vec3& aVec);

		// Operator Overloads w/ a scalar
		vec3 operator*(const float& scalar) const;
		vec3& operator*=(const float& scalar);

		vec3 operator/(const float& scalar) const;
		vec3& operator/=(const float& scalar);
		
		// Boolean operators
		bool operator==(const vec3& aVec) const;
		bool operator!=(const vec3& aVec) const;
		bool operator>=(const vec3& aVec) const; // Compares lengths
		bool operator>(const vec3& aVec) const;  // Compares lengths
		bool operator<=(const vec3& aVec) const; // Compares lengths
		bool operator<(const vec3& aVec) const;  // Compares lengths

		// Cast operator overload
		operator vec2() const { return vec2(this->x, this->y); }

		// ostream Operator
		friend std::ostream& operator<<(std::ostream& os, const vec3& vec);

		// Length Calculation
		float length() const;

		// Common Swizzles
		vec2 xy() { return vec2(x, y); }
		vec2 yz() { return vec2(y, z); }
		vec2 xz() { return vec2(x, z); }

		// Member Variables
		float x;
		float y;
		float z;
	};

	struct vec4
	{
		// Constructors
		vec4() : x(0), y(0), z(0), w(0) {}
		vec4(const int& x, const int& y, const int& z, const int& w) : x(x), y(y), z(z), w(w) {}
		vec4(const float& x, const float& y, const float& z, const float& w) : x(x), y(y), z(z), w(w) {}
		vec4(const float& value) : x(value), y(value), z(value), w(value) {}
		vec4(const int& value) : x(value), y(value), z(value), w(value) {}

		vec4(const vec2& vecA, const vec2& vecB) : x(vecA.x), y(vecA.y), z(vecB.x), w(vecB.y) {}
		vec4(const vec2& vecA, const float& z, const float& w) : x(vecA.x), y(vecA.y), z(z), w(w) {}
		vec4(const float& x, const float& y, const vec2& vecA) : x(x), y(y), z(vecA.x), w(vecA.y) {}
		vec4(const vec3& vec, const float& w = 0.0f) : x(vec.x), y(vec.y), z(vec.z), w(w) {}
		vec4(const vec4& vec) : x(vec.x), y(vec.y), z(vec.z), w(vec.w) {} // Copy Constructor

		// Operator Overloads w/ other vector
		vec4 operator+(const vec4& aVec) const;
		vec4& operator+=(const vec4& aVec);

		vec4 operator-(const vec4& aVec) const;
		vec4& operator-=(const vec4& aVec);

		vec4 operator*(const vec4& aVec) const;
		vec4& operator*=(const vec4& aVec);

		vec4 operator/(const vec4& aVec) const;
		vec4& operator/=(const vec4& aVec);
		
		vec4& operator=(const vec4& aVec);

		// Operator Overloads w/ a scalar
		vec4 operator*(const float& scalar) const;
		vec4& operator*=(const float& scalar);

		vec4 operator/(const float& scalar) const;
		vec4& operator/=(const float& scalar);

		// Boolean operators
		bool operator==(const vec4& aVec) const;
		bool operator!=(const vec4& aVec) const;
		bool operator>=(const vec4& aVec) const; // Compares lengths
		bool operator>(const vec4& aVec) const;  // Compares lengths
		bool operator<=(const vec4& aVec) const; // Compares lengths
		bool operator<(const vec4& aVec) const;  // Compares lengths

		// Cast operator overload
		operator vec2() const { return vec2(this->x, this->y); }
		operator vec3() const { return vec3(this->x, this->y, this->z); }

		// ostream Operator
		friend std::ostream& operator<<(std::ostream& os, const vec4& vec);

		// Length Calculation
		float length() const;

		// Member Variables
		float x;
		float y;
		float z;
		float w;
	};


	// * ------------------ *
	// |  Vector Functions  |
	// * ------------------ *

	// Dot Product
	float dot(vec2 vecA, vec2 vecB);
	float dot(vec3 vecA, vec3 vecB);
	float dot(vec4 vecA, vec4 vecB);

	// Cross Product
	vec2 cross(vec2 vecA, vec2 vecB);
	vec3 cross(vec3 vecA, vec3 vecB);
	vec4 cross(vec4 vecA, vec4 vecB);

	// Normalize
	vec2 normalize(vec2 vec);
	vec3 normalize(vec3 vec);
	vec4 normalize(vec4 vec);
}

#endif // !VECTORS