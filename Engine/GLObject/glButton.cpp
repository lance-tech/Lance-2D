#include "glButton.h"


glButton::glButton() : glWidget()
{
}


glButton::glButton(std::string text) : glButton()
{
	SetText(text);
}


glButton::~glButton()
{
}
