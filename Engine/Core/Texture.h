#pragma once

#include <string>
#include <GL/glew.h>

class Texture
{
public:
	Texture() : ID(0), SamplerID(0), Path()
	{
	}

	~Texture() {}

	GLuint ID;
	GLuint SamplerID;
	std::string Path;
};