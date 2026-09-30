#include "color.h"


// Operator definitions
Color& Color::operator=(const Color& color)
{
	this->color = color.color; return *this;
}
Color Color::operator*(const Color& color)
{
	return Color(color.r() * this->color.x, color.g() * this->color.y, color.b() * this->color.z, color.a() * this->color.w);
}
Color& Color::operator*=(const Color& color)
{
	this->color.x *= color.r();
	this->color.y *= color.g();
	this->color.z *= color.b(); 
	this->color.w *= color.a(); 
	return *this;
}