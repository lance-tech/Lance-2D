#pragma once

#include <cmath>
#include <map>
#include <string>

#include <GL/glew.h>

// GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/euler_angles.hpp>
//using namespace glm;

// FreeType
#include <ft2build.h>
#include FT_FREETYPE_H

#include "../Controller/MaterialManager.h"
#include "../Core/Shader.h"
#include "../Core/Transform.h"
#include "../Core/Color.h"

struct Character {
	GLuint TextureID;   // ID handle of the glyph texture
	glm::ivec2 Size;    // Size of glyph
	glm::ivec2 Bearing;  // Offset from baseline to left/top of glyph
	GLuint Advance;    // Horizontal offset to advance to next glyph
};


class glText
{
public:
	glText();
	glText(std::string text);
	~glText();

	Color color;
	Core::Transform transform;

	void SetText(std::string text);
	std::string GetText();
	float GetTextWidth();
	float GetTextHeight();
	int GetLength();
	GLfloat GetFontSize();
	void SetFontSize(GLfloat size);
	virtual void Draw(glm::mat4 &VP, double time = 0);

private:
	GLuint MatrixID;
	GLuint TextColorID;

	std::map<GLchar, Character> Characters;
	GLuint VAO, VBO;

	GLfloat scale;
	GLfloat fontSize;

	std::string text;
	Shader* shader;
};