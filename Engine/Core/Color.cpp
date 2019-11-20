#include "Color.h"



Color::Color(float r, float g, float b, float a) : Color()
{
	SetColor(r, g, b, a);
}

Color::Color():
	R(1.0f), G(1.0f), B(1.0f), A(1.0f), Specular(0.65f)
{}


Color::~Color()
{
}

void Color::SetColor(float r, float g, float b, float a)
{
	R = r;
	G = g;
	B = b;
	A = a;
}

Color& Color::GetColor()
{
	return *this;
}
