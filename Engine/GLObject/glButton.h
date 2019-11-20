#pragma once

#include <string>
#include "glWidget.h"

class glButton : public glWidget
{
public:
	glButton();
	glButton(std::string text);
	~glButton();
};

