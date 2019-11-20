#pragma once

#include <string>
#include "glWidget.h"

class glLabel : public glWidget
{
public:
	glLabel();
	glLabel(std::string text);
	~glLabel();
};

