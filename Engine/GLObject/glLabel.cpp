#include "glLabel.h"

glLabel::glLabel() : glWidget()
{
}

glLabel::glLabel(std::string text) : glLabel()
{
	SetText(text);
}

glLabel::~glLabel()
{
}
