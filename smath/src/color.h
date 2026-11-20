#ifndef COLOR
#pragma once

#include <iostream>
#include "vectors.h"
#include "conversions.h"
#include "clamping.h"

namespace smath
{
	class Color
	{
	public:
		// Constructors
		Color() : color(smath::vec4(1)) {}
		Color(float r, float g, float b, float a = 1.0f) : color(smath::vec4(r, g, b, a)) {}
		Color(smath::vec3 color, float a = 1.0f) : color(smath::vec4(color, a)) {}
		Color(smath::vec4 color) : color(color) {}
		Color(float color, float a = 1.0f) : color(smath::vec4(smath::vec3(color), a)) {}

		// Getters
		smath::vec3 rgb() const { return smath::vec3(color); }
		smath::vec4 rgba() const { return color; };
		smath::vec3 hsv() const { return RGBtoHSV(color); }
		smath::vec4 hsva() const { return RGBAtoHSVA(color); }
		float alpha() const { return color.w; }
		std::string hex() const { return RGBAtoHEX(color); }

		// Float Channel Getters
		float r() const { return color.x; }
		float g() const { return color.y; }
		float b() const { return color.z; }
		float h() const { return hsv().x; }
		float s() const { return hsv().y; }
		float v() const { return hsv().z; }
		float a() const { return color.w; }

		// Int Channel Getters
		int intR() const { return (int)(color.x * 255.0f); }
		int intG() const { return (int)(color.y * 255.0f); }
		int intB() const { return (int)(color.z * 255.0f); }
		int intH() const { return (int)(hsv().x * 255.0f); }
		int intS() const { return (int)(hsv().y * 255.0f); }
		int intV() const { return (int)(hsv().z * 255.0f); }
		int intA() const { return (int)(color.w * 255.0f); }

		// Setters
		void setRGB(const smath::vec3 rgb) { color = smath::vec4(rgb, color.w); }
		void setRGBA(const smath::vec4 rgba) { color = rgba; }
		void setHSV(const smath::vec3 hsv) { color = smath::vec4(HSVtoRGB(hsv), color.w); }
		void setHSVA(const smath::vec4 hsva) { color = HSVAtoRGBA(hsva); }
		void setHue(const float hue) { setHSV(smath::vec3(hue, s(), v())); }
		void setSaturation(const float saturation) { setHSV(smath::vec3(h(), saturation, v())); }
		void setValue(const float value) { setHSV(smath::vec3(h(), s(), value)); }
		void setAlpha(const float alpha) { color.w = alpha; }
		bool setHex(std::string hex);

		// RGB <--> HSV Functions
		static smath::vec3 RGBtoHSV(smath::vec3 rgb);
		static smath::vec4 RGBAtoHSVA(smath::vec4 rgba);
		static smath::vec3 HSVtoRGB(smath::vec3 hsv);
		static smath::vec4 HSVAtoRGBA(smath::vec4 hsva);

		// RGB & HSV <--> Hex Functions
		static std::string RGBtoHEX(smath::vec3 rgb);
		static std::string RGBAtoHEX(smath::vec4 rgba);
		static std::string HSVtoHEX(smath::vec3 hsv);
		static std::string HSVAtoHEX(smath::vec4 hsva);
		static smath::vec3 HEXtoRGB(std::string hex);
		static smath::vec4 HEXtoRGBA(std::string hex);
		static smath::vec3 HEXtoHSV(std::string hex);
		static smath::vec4 HEXtoHSVA(std::string hex);

		// Operators
		Color& operator=(const Color& color);
		Color operator*(const Color& color);
		Color& operator*=(const Color& color);

	private:
		smath::vec4 color;
	};
}

#endif // !COLOR