#include "color.h"

using namespace smath;

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

// Setters
bool Color::setHex(std::string hex)
{
	// Erasing hashtag symbol (if it exists)
	if (hex[0] == '#')
	{
		hex.erase(hex.begin());
	}

	// Checking if hex value has alpha 
	if (hex.length() == 8) // alpha
	{
		smath::vec4 testConvert = HEXtoRGBA(hex);
		if (testConvert == smath::vec4(-1))
		{
			color = rgba();
			return false; // Returning false for failed conversion
		}
		else
		{
			color = testConvert;
			return true; // Returning true for successful conversion
		}

	}
	else if (hex.length() == 6) // no alpha
	{
		smath::vec3 testConvert = HEXtoRGB(hex);
		if (testConvert == smath::vec3(-1))
		{
			color = rgba();
			return false; // Returning false for failed conversion
		}
		else
		{
			color = smath::vec4(testConvert, alpha());;
			return true; // Returning true for successful conversion

		}
	}

	// Returning false for improper length
	return false;
}

// RGB <--> HSV Functions
smath::vec3 Color::RGBtoHSV(smath::vec3 rgb)
{
	// Necessary Values for 
	const float chromaMax = smath::max(rgb.x, rgb.y, rgb.z);
	const float chromaMin = smath::min(rgb.x, rgb.y, rgb.z);
	const float chromaDelta = chromaMax - chromaMin;

	// Calculating saturation
	const float saturation = (chromaMax <= 0.0f ? 0 : chromaDelta / chromaMax);

	// Calculating saturation
	const float value = chromaMax;

	// Calculating hue
	float hue = 0.0f;
	if (chromaDelta <= 0.0f) // NaN
		return smath::vec3(hue, saturation, value);
	if (chromaMax == rgb.x)
		hue = 60.0f * fmod((rgb.y - rgb.z) / chromaDelta, 6.0f);
	else if (chromaMax == rgb.y)
		hue = 60.0f * (((rgb.z - rgb.x) / chromaDelta) + 2.0f);
	else if (chromaMax == rgb.z)
		hue = 60.0f * (((rgb.x - rgb.y) / chromaDelta) + 4.0f);

	// Making sure hue is above 0
	if (hue < 0.0f)
	{
		hue += 360.0f;
	}

	// Converting hue to 0-1
	hue /= 360.0f;

	return smath::vec3(hue, saturation, value);
}
smath::vec4 Color::RGBAtoHSVA(smath::vec4 rgba)
{ 
	return smath::vec4(RGBtoHSV(rgba), rgba.w); 
}
smath::vec3 Color::HSVtoRGB(smath::vec3 hsv)
{
	// Getting hue (and other variables)
	const float hue = hsv.x * 360.0f;
	const float chroma = hsv.y * hsv.z;
	const float huePrime = fmod(hue / 60.0f, 6.0f);
	const float x = chroma * (1.0f - abs(fmod(huePrime, 2.0f) - 1.0f));
	const float m = hsv.z - chroma;

	// Calculating rgb
	smath::vec3 rgb = smath::vec3(0);

	if (huePrime <= 1.0f)
		rgb = smath::vec3(chroma, x, 0.0f);
	else if (huePrime <= 2.0f)
		rgb = smath::vec3(x, chroma, 0.0f);
	else if (huePrime <= 3.0f)
		rgb = smath::vec3(0.0f, chroma, x);
	else if (huePrime <= 4.0f)
		rgb = smath::vec3(0.0f, x, chroma);
	else if (huePrime <= 5.0f)
		rgb = smath::vec3(x, 0.0f, chroma);
	else if (huePrime <= 6.0f)
		rgb = smath::vec3(chroma, 0.0f, x);

	return rgb + smath::vec3(m);
}
smath::vec4 Color::HSVAtoRGBA(smath::vec4 hsva)
{ 
	return smath::vec4(HSVtoRGB(hsva), hsva.w); 
}

// RGB & HSV <--> Hex Functions
std::string Color::RGBtoHEX(smath::vec3 rgb)
{
	// Converting floats to integer values between 0 and 255
	int r = static_cast<int>(smath::clamp(rgb.x, 0.0f, 1.0f) * 255);
	int g = static_cast<int>(smath::clamp(rgb.y, 0.0f, 1.0f) * 255);
	int b = static_cast<int>(smath::clamp(rgb.z, 0.0f, 1.0f) * 255);

	std::string result = '#' + smath::decToHexa(r, 2) + smath::decToHexa(g, 2) + smath::decToHexa(b, 2);
	return result;
}
std::string Color::RGBAtoHEX(smath::vec4 rgba)
{
	// Converting floats to integer values between 0 and 255
	int r = static_cast<int>(smath::clamp(rgba.x, 0.0f, 1.0f) * 255);
	int g = static_cast<int>(smath::clamp(rgba.y, 0.0f, 1.0f) * 255);
	int b = static_cast<int>(smath::clamp(rgba.z, 0.0f, 1.0f) * 255);
	int a = static_cast<int>(smath::clamp(rgba.w, 0.0f, 1.0f) * 255);

	std::string result = '#' + smath::decToHexa(r, 2) + smath::decToHexa(g, 2) + smath::decToHexa(b, 2) + smath::decToHexa(a, 2);
	return result;
}
std::string Color::HSVtoHEX(smath::vec3 hsv)
{
	// Converting to RGB then converting to Hex
	return RGBtoHEX(HSVtoRGB(hsv));
}
std::string Color::HSVAtoHEX(smath::vec4 hsva)
{
	// Converting to RGBA then converting to Hex
	return RGBAtoHEX(HSVAtoRGBA(hsva));
}
smath::vec3 Color::HEXtoRGB(std::string hex)
{
	// Checking if first character is a hashtag, then removing it
	if (hex[0] == '#')
	{
		hex.erase(hex.begin());
	}

	// Checking if there are the correct amount of hexadecimal digits
	if (hex.size() != 6)
		return smath::vec3(-1); // -1 used as error value, since RGB is 0-1

	// Value the color will be stored in
	int decimalValues[3];

	// Looping through each digit pair
	for (int i = 0; i < 3; i++)
	{
		// Getting the individual hex values
		int hexIndex = i * 2;
		std::string tempHex = { hex[hexIndex], hex[hexIndex + 1] };

		// Converting the hex values
		decimalValues[i] = smath::hexaToDec(tempHex);
	}

	// Returning the output
	return smath::vec3((float)decimalValues[0], (float)decimalValues[1], (float)decimalValues[2]) * (1.0f / 255.0f);
}
smath::vec4 Color::HEXtoRGBA(std::string hex)
{
	// Checking if first character is a hashtag, then removing it
	if (hex[0] == '#')
	{
		hex.erase(hex.begin());
	}

	// Checking if there are the correct amount of hexadecimal digits
	if (hex.size() != 8)
		return smath::vec4(-1); // -1 used as error value, since RGB is 0-1

	// Value the color will be stored in
	int decimalValues[4];

	// Looping through each digit pair
	for (int i = 0; i < 4; i++)
	{
		// Getting the individual hex values
		int hexIndex = i * 2;
		std::string tempHex = { hex[hexIndex], hex[hexIndex + 1] };

		// Converting the hex values
		decimalValues[i] = smath::hexaToDec(tempHex);
	}

	// Returning the output
	return smath::vec4((float)decimalValues[0], (float)decimalValues[1], (float)decimalValues[2], (float)decimalValues[3]) * (1.0f / 255.0f);
}
smath::vec3 Color::HEXtoHSV(std::string hex)
{
	// Converting from hex to rgb, then to hsv
	return RGBtoHSV(HEXtoRGB(hex));
}
smath::vec4 Color::HEXtoHSVA(std::string hex)
{
	// Converting from hex to rgba, then to hsva
	return RGBAtoHSVA(HEXtoRGBA(hex));
}