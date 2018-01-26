#include "Engine/Core/Rgba.hpp"

//*******************************************************************
const Rgba Rgba::WHITE = Rgba(255, 255, 255, 255);

//*******************************************************************
const Rgba Rgba::BLACK = Rgba(0, 0, 0, 255);

//*******************************************************************
const Rgba Rgba::YELLOW = Rgba(255, 255, 0, 255);

//*******************************************************************
const Rgba Rgba::GREEN = Rgba(0, 255, 0, 255);

//*******************************************************************
const Rgba Rgba::RED = Rgba(255, 0, 0, 255);

//*******************************************************************
const Rgba Rgba::BLUE = Rgba(0, 0, 255, 255);

//*******************************************************************
const Rgba Rgba::MAGENTA = Rgba(255, 0, 255, 255);

//*******************************************************************
const Rgba Rgba::CYAN = Rgba(0, 255, 255, 255);

//*******************************************************************
const Rgba Rgba::MEDIUM_GREY = Rgba(125, 125, 125, 255);

//*******************************************************************
const Rgba Rgba::LIGHT_BLUE = Rgba(66, 134, 244, 255);

//*******************************************************************
const Rgba Rgba::PURPLE = Rgba(102, 51, 153, 255);

//*******************************************************************
const Rgba Rgba::ORANGE = Rgba(255, 165, 0, 255);

//*******************************************************************
const Rgba Rgba::TRANSPRNT = Rgba(0, 0, 0, 0);

//*******************************************************************
Rgba::Rgba()
	: r(255)
	, g(255)
	, b(255)
	, a(255)
{
}

//*******************************************************************
Rgba::~Rgba()
{
}

//*******************************************************************
Rgba::Rgba(unsigned char redByte, unsigned char greenByte, unsigned char blueByte, unsigned char alphaByte)
	: r(redByte)
	, g(greenByte)
	, b(blueByte)
	, a(alphaByte)
{
}

//*******************************************************************
//Rgba::Rgba(float normalizedRed, float normalizedGreen, float normalizedBlue, float normalizedAlpha)
//	: r(ConvertFloatColorToByteColor(normalizedRed))
//	, g(ConvertFloatColorToByteColor(normalizedGreen))
//	, b(ConvertFloatColorToByteColor(normalizedBlue))
//	, a(ConvertFloatColorToByteColor(normalizedAlpha))
//{
//}

//*******************************************************************
void Rgba::SetAsBytes(unsigned char redByte, unsigned char greenByte, unsigned char blueByte, unsigned char alphaByte)
{
	r = redByte;
	g = greenByte;
	b = blueByte;
	a = alphaByte;
}

//*******************************************************************
void Rgba::SetAsFloats(float normalizedRed, float normalizedGreen, float normalizedBlue, float normalizedAlpha)
{
	r = ConvertFloatColorToByteColor(normalizedRed);
	g = ConvertFloatColorToByteColor(normalizedGreen);
	b = ConvertFloatColorToByteColor(normalizedBlue);
	a = ConvertFloatColorToByteColor(normalizedAlpha);
}

//*******************************************************************
void Rgba::GetAsFloats(float& out_normalizedRed, float& out_normalizedGreen, float& out_normalizedBlue, float& out_normalizedAlpha) const
{
	float inverse255 = 1.f / 255.f;
	out_normalizedRed = (float)r*inverse255;
	out_normalizedGreen = (float)g*inverse255;
	out_normalizedBlue = (float)b*inverse255;
	out_normalizedAlpha = (float)a*inverse255;
}

//*******************************************************************
void Rgba::ScaleRGB(float rgbScale)
{
	r = ConvertFloatColorToByteColor(Normalize(r)*rgbScale);
	g = ConvertFloatColorToByteColor(Normalize(g)*rgbScale);
	b = ConvertFloatColorToByteColor(Normalize(b)*rgbScale);
}

//*******************************************************************
void Rgba::ScaleAlpha(float alphaScale)
{
	a = ConvertFloatColorToByteColor(Normalize(a)*alphaScale);
}

//*******************************************************************
void Rgba::SubtractRGB(float rgbSubtract)
{
	r = ConvertFloatColorToByteColor(Normalize(r) - rgbSubtract);
	g = ConvertFloatColorToByteColor(Normalize(g) - rgbSubtract);
	b = ConvertFloatColorToByteColor(Normalize(b) - rgbSubtract);
}

//*******************************************************************
void Rgba::SubtractAlpha(float alphaSubtract)
{
	a = ConvertFloatColorToByteColor(Normalize(a) - alphaSubtract);
}

//*******************************************************************
void Rgba::AddAlpha(float alphaAdd)
{
	a = ConvertFloatColorToByteColor(Normalize(a) + alphaAdd);
}

//*******************************************************************
unsigned char Rgba::ConvertFloatColorToByteColor(float normalizedColor)
{
	int colorInt = (int)(normalizedColor * 255);
	if (colorInt > 255)
		colorInt = 255;
	if (colorInt < 0)
		colorInt = 0;
	return (unsigned char)colorInt;
}

//*******************************************************************
float Rgba::Normalize(unsigned char colorByte)
{
	float inverse255 = 1.f / 255.f;
	return colorByte*inverse255;
}

//*******************************************************************
bool Rgba::operator==(const Rgba& rgbaToEqual) const
{
	return ((r == rgbaToEqual.r) && (g == rgbaToEqual.g) && (b == rgbaToEqual.b) && (a == rgbaToEqual.a));
}

//*******************************************************************
bool Rgba::operator!=(const Rgba& rgbaToNotEqual) const
{
	return !(*(this) == rgbaToNotEqual);
}

