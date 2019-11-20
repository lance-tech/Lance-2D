#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <GL/glew.h>

namespace ImageLoader
{
	GLuint BMP(const char * imagepath);
	GLuint DDS(const char * imagepath);
}