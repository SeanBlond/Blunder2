#include "vectors.h"
#include "ivectors.h"
namespace smath
{
	// * ---------------- *
	// |  vec2 functions  |
	// * ---------------- *

	// Operator Overloads w/ other vector
	vec2 vec2::operator+(const vec2& aVec) const { return vec2(this->x + aVec.x, this->y + aVec.y); }
	vec2& vec2::operator+=(const vec2& aVec) { this->x += aVec.x; this->y += aVec.y; return *this; }

	vec2 vec2::operator-(const vec2& aVec) const { return vec2(this->x - aVec.x, this->y - aVec.y); }
	vec2& vec2::operator-=(const vec2& aVec) { this->x -= aVec.x; this->y -= aVec.y; return *this; }

	vec2 vec2::operator*(const vec2& aVec) const { return vec2(this->x * aVec.x, this->y * aVec.y); }
	vec2& vec2::operator*=(const vec2& aVec) { this->x *= aVec.x; this->y *= aVec.y; return *this; }

	vec2 vec2::operator/(const vec2& aVec) const { return vec2(this->x / aVec.x, this->y / aVec.y); }
	vec2& vec2::operator/=(const vec2& aVec) { this->x /= aVec.x; this->y /= aVec.y; return *this; }

	vec2& vec2::operator=(const vec2& aVec) { this->x = aVec.x; this->y = aVec.y; return *this; }

	// Operator Overloads w/ a scalar
	vec2 vec2::operator*(const float& scalar) const { return vec2(this->x * scalar, this->y * scalar); }
	vec2& vec2::operator*=(const float& scalar) { this->x *= scalar; this->y *= scalar; return *this; }

	vec2 vec2::operator/(const float& scalar) const { return vec2(this->x / scalar, this->y / scalar); }
	vec2& vec2::operator/=(const float& scalar) { this->x /= scalar; this->y /= scalar; return *this; }

	// Boolean operators
	bool vec2::operator==(const vec2& aVec) const { return (this->x == aVec.x) && (this->y == aVec.y); }
	bool vec2::operator!=(const vec2& aVec) const { return (this->x != aVec.x) || (this->y != aVec.y); }
	bool vec2::operator>=(const vec2& aVec) const { return length() >= aVec.length(); }
	bool vec2::operator>(const vec2& aVec) const  { return length() >  aVec.length(); }
	bool vec2::operator<=(const vec2& aVec) const { return length() <= aVec.length(); }
	bool vec2::operator<(const vec2& aVec) const  { return length() <  aVec.length(); }

	// Length Calculation
	float vec2::length() const { return std::sqrt((x * x) + (y * y)); }

	// Conversion Operators
	vec2::operator ivec2() const
	{
		return ivec2((int)x, (int)y);
	}


	// * ---------------- *
	// |  vec3 functions  |
	// * ---------------- *

	// Operator Overloads w/ other vector
	vec3 vec3::operator+(const vec3& aVec) const { return vec3(this->x + aVec.x, this->y + aVec.y, this->z + aVec.z); }
	vec3& vec3::operator+=(const vec3& aVec) { this->x += aVec.x; this->y += aVec.y; this->z += aVec.z; return *this; }

	vec3 vec3::operator-(const vec3& aVec) const { return vec3(this->x - aVec.x, this->y - aVec.y, this->z - aVec.z); }
	vec3& vec3::operator-=(const vec3& aVec) { this->x -= aVec.x; this->y -= aVec.y; this->z -= aVec.z; return *this; }

	vec3 vec3::operator*(const vec3& aVec) const { return vec3(this->x * aVec.x, this->y * aVec.y, this->z * aVec.z); }
	vec3& vec3::operator*=(const vec3& aVec) { this->x *= aVec.x; this->y *= aVec.y; this->z *= aVec.z; return *this; }

	vec3 vec3::operator/(const vec3& aVec) const { return vec3(this->x / aVec.x, this->y / aVec.y, this->z / aVec.z); }
	vec3& vec3::operator/=(const vec3& aVec) { this->x /= aVec.x; this->y /= aVec.y; this->z /= aVec.z; return *this; }

	vec3& vec3::operator=(const vec3& aVec) { this->x = aVec.x; this->y = aVec.y; this->z = aVec.z; return *this; }

	// Operator Overloads w/ a scalar
	vec3 vec3::operator*(const float& scalar) const { return vec3(this->x * scalar, this->y * scalar, this->z * scalar); }
	vec3& vec3::operator*=(const float& scalar) { this->x *= scalar; this->y *= scalar; this->z *= scalar; return *this; }

	vec3 vec3::operator/(const float& scalar) const { return vec3(this->x / scalar, this->y / scalar, this->z / scalar); }
	vec3& vec3::operator/=(const float& scalar) { this->x /= scalar; this->y /= scalar; this->z /= scalar; return *this; }

	// Boolean operators
	bool vec3::operator==(const vec3& aVec) const { return (this->x == aVec.x) && (this->y == aVec.y) && (this->z == aVec.z); }
	bool vec3::operator!=(const vec3& aVec) const { return (this->x != aVec.x) || (this->y != aVec.y) || (this->z != aVec.z); }
	bool vec3::operator>=(const vec3& aVec) const { return length() >= aVec.length(); }
	bool vec3::operator>(const vec3& aVec) const  { return length() >  aVec.length(); }
	bool vec3::operator<=(const vec3& aVec) const { return length() <= aVec.length(); }
	bool vec3::operator<(const vec3& aVec) const  { return length() <  aVec.length(); }

	// Length Calculation
	float vec3::length() const { return std::sqrt((x * x) + (y * y) + (z * z)); }


	// * ---------------- *
	// |  vec4 functions  |
	// * ---------------- *

	// Operator Overloads w/ other vector
	vec4 vec4::operator+(const vec4& aVec) const { return vec4(this->x + aVec.x, this->y + aVec.y, this->z + aVec.z, this->w + aVec.w); }
	vec4& vec4::operator+=(const vec4& aVec) { this->x += aVec.x; this->y += aVec.y; this->z += aVec.z; this->w += aVec.w; return *this; }

	vec4 vec4::operator-(const vec4& aVec) const { return vec4(this->x - aVec.x, this->y - aVec.y, this->z - aVec.z, this->w - aVec.w); }
	vec4& vec4::operator-=(const vec4& aVec) { this->x -= aVec.x; this->y -= aVec.y; this->z -= aVec.z; this->w -= aVec.w; return *this; }

	vec4 vec4::operator*(const vec4& aVec) const { return vec4(this->x * aVec.x, this->y * aVec.y, this->z * aVec.z, this->w * aVec.w); }
	vec4& vec4::operator*=(const vec4& aVec) { this->x *= aVec.x; this->y *= aVec.y; this->z *= aVec.z; this->w *= aVec.w; return *this; }

	vec4 vec4::operator/(const vec4& aVec) const { return vec4(this->x / aVec.x, this->y / aVec.y, this->z / aVec.z, this->w / aVec.w); }
	vec4& vec4::operator/=(const vec4& aVec) { this->x /= aVec.x; this->y /= aVec.y; this->z /= aVec.z; this->w /= aVec.w; return *this; }

	vec4& vec4::operator=(const vec4& aVec) { this->x = aVec.x; this->y = aVec.y; this->z = aVec.z; this->w = aVec.w; return *this; }

	// Operator Overloads w/ a scalar
	vec4 vec4::operator*(const float& scalar) const { return vec4(this->x * scalar, this->y * scalar, this->z * scalar, this->w * scalar); }
	vec4& vec4::operator*=(const float& scalar) { this->x *= scalar; this->y *= scalar; this->z *= scalar; this->w *= scalar; return *this; }

	vec4 vec4::operator/(const float& scalar) const { return vec4(this->x / scalar, this->y / scalar, this->z / scalar, this->w / scalar); }
	vec4& vec4::operator/=(const float& scalar) { this->x /= scalar; this->y /= scalar; this->z /= scalar; this->w /= scalar; return *this; }

	// Boolean operators
	bool vec4::operator==(const vec4& aVec) const { return (this->x == aVec.x) && (this->y == aVec.y) && (this->z == aVec.z) && (this->w == aVec.w); }
	bool vec4::operator!=(const vec4& aVec) const { return (this->x != aVec.x) || (this->y != aVec.y) || (this->z != aVec.z) || (this->w != aVec.w); }
	bool vec4::operator>=(const vec4& aVec) const { return length() >= aVec.length(); }
	bool vec4::operator>(const vec4& aVec) const  { return length() >  aVec.length(); } 
	bool vec4::operator<=(const vec4& aVec) const { return length() <= aVec.length(); }
	bool vec4::operator<(const vec4& aVec) const  { return length() <  aVec.length(); }

	// Length Calculation
	float vec4::length() const { return std::sqrt((x * x) + (y * y) + (z * z) + (w * w)); }


	// * ------------------ *
	// |  Vector Functions  |
	// * ------------------ *

	// Friend ostream overload functions
	std::ostream& operator<<(std::ostream& os, const vec2& vec)
	{
		os << '(' << vec.x << ", " << vec.y << ')';
		return os;
	}
	std::ostream& operator<<(std::ostream& os, const vec3& vec)
	{
		os << '(' << vec.x << ", " << vec.y << ", " << vec.z << ')';
		return os;
	}
	std::ostream& operator<<(std::ostream& os, const vec4& vec)
	{
		os << '(' << vec.x << ", " << vec.y << ", " << vec.z << ", " << vec.w << ')';
		return os;
	}

	// Dot Product
	float dot(vec2 vecA, vec2 vecB)
	{
		return (vecA.x * vecB.x) + (vecA.y * vecB.y);
	}
	float dot(vec3 vecA, vec3 vecB)
	{
		return (vecA.x * vecB.x) + (vecA.y * vecB.y) + (vecA.z * vecB.z);
	}
	float dot(vec4 vecA, vec4 vecB)
	{
		return (vecA.x * vecB.x) + (vecA.y * vecB.y) + (vecA.z * vecB.z) + (vecA.w * vecB.w);
	}

	// Cross Product
	vec2 cross(vec2 vecA, vec2 vecB)
	{
		// NEEDS TO BE IMPLEMENTED
		return vec2(-1);
	}
	vec3 cross(vec3 vecA, vec3 vecB)
	{
		// NEEDS TO BE IMPLEMENTED
		return vec3(-1);
	}
	vec4 cross(vec4 vecA, vec4 vecB)
	{
		// NEEDS TO BE IMPLEMENTED
		return vec4(-1);
	}

	// Normalize
	vec2 normalize(vec2 vec)
	{
		return vec / vec.length();
	}
	vec3 normalize(vec3 vec)
	{
		return vec / vec.length();
	}
	vec4 normalize(vec4 vec)
	{
		return vec / vec.length();
	}

}