#pragma once

class Color
{
public:
	Color(float r, float g, float b, float a = 1.0f);
	Color();
	~Color();

	Color& operator=(const Color &other)
	{
		this->R = other.R;
		this->G = other.G;
		this->B = other.B;
		this->A = other.A;
		this->Specular = other.Specular;
		return *this;
	}

	float R;
	float G;
	float B;
	float A;

	float Specular;

	void SetColor(float r, float g, float b, float a = 1.0f);
	Color& GetColor();

	//void SetSelectedColor(bool isSelected);
};

