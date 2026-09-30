#include "ivectors.h"
namespace smath
{
	// * ----------------- *
	// |  ivec2 functions  |
	// * ----------------- *

	// Operator Overloads w/ other vector
	ivec2 ivec2::operator+(const ivec2& aVec) const { return ivec2(this->x + aVec.x, this->y + aVec.y); }
	ivec2& ivec2::operator+=(const ivec2& aVec) { this->x += aVec.x; this->y += aVec.y; return *this; }

	ivec2 ivec2::operator-(const ivec2& aVec) const { return ivec2(this->x - aVec.x, this->y - aVec.y); }
	ivec2& ivec2::operator-=(const ivec2& aVec) { this->x -= aVec.x; this->y -= aVec.y; return *this; }

	ivec2 ivec2::operator*(const ivec2& aVec) const { return ivec2(this->x * aVec.x, this->y * aVec.y); }
	ivec2& ivec2::operator*=(const ivec2& aVec) { this->x *= aVec.x; this->y *= aVec.y; return *this; }

	ivec2 ivec2::operator/(const ivec2& aVec) const { return ivec2(this->x / aVec.x, this->y / aVec.y); }
	ivec2& ivec2::operator/=(const ivec2& aVec) { this->x /= aVec.x; this->y /= aVec.y; return *this; }

	ivec2& ivec2::operator=(const ivec2& aVec) { this->x = aVec.x; this->y = aVec.y; return *this; }

	// Operator Overloads w/ a scalar
	ivec2 ivec2::operator*(const int& scalar) const { return ivec2(this->x * scalar, this->y * scalar); }
	ivec2& ivec2::operator*=(const int& scalar) { this->x *= scalar; this->y *= scalar; return *this; }

	ivec2 ivec2::operator/(const int& scalar) const { return ivec2(this->x / scalar, this->y / scalar); }
	ivec2& ivec2::operator/=(const int& scalar) { this->x /= scalar; this->y /= scalar; return *this; }

	// Boolean operators
	bool ivec2::operator==(const ivec2& aVec) const { return (this->x == aVec.x) && (this->y == aVec.y); }
	bool ivec2::operator!=(const ivec2& aVec) const { return (this->x != aVec.x) || (this->y != aVec.y); }


	// * ------------------ *
	// |  Vector Functions  |
	// * ------------------ *

	std::ostream& operator<<(std::ostream& os, const ivec2& vec)
	{
		os << '(' << vec.x << ", " << vec.y << ')';
		return os;
	}
}