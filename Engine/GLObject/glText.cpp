#include "glText.h"


glText::glText() 
	: shader(nullptr), text("Text")
{
	shader = &MaterialManager::CreateShader("Assets/shaders/text.vert", "Assets/shaders/text.frag");

	color.SetColor(0.0f, 0.0f, 0.0f);
	SetFontSize(1);

	// Compile and setup the shader
	MatrixID = glGetUniformLocation(shader->GetProgram(), "MVP");
	TextColorID = glGetUniformLocation(shader->GetProgram(), "inputColor");

	// FreeType
	FT_Library ft;
	// All functions return a value different than 0 whenever an error occurred
	if (FT_Init_FreeType(&ft))
		std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;

	// Load font as face
	FT_Face face;
	if (FT_New_Face(ft, "Assets/fonts/ARLRDBD.TTF", 0, &face))
		std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;

	// Set size to load glyphs as
	//FT_Set_Pixel_Sizes(face, 0, 48);
	FT_Set_Pixel_Sizes(face, 0, 48);

	// Disable byte-alignment restriction
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	// Load first 128 characters of ASCII set
	for (GLubyte c = 0; c < 128; c++)
	{
		// Load character glyph 
		if (FT_Load_Char(face, c, FT_LOAD_RENDER))
		{
			std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
			continue;
		}
		// Generate texture
		GLuint texture;
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);
		glTexImage2D(
			GL_TEXTURE_2D,
			0,
			GL_RED,
			face->glyph->bitmap.width,
			face->glyph->bitmap.rows,
			0,
			GL_RED,
			GL_UNSIGNED_BYTE,
			face->glyph->bitmap.buffer
		);
		// Set texture options
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		// Now store character for later use
		Character character = {
			texture,
			glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
			glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
			(GLuint)face->glyph->advance.x
		};
		Characters.insert(std::pair<GLchar, Character>(c, character));
	}
	glBindTexture(GL_TEXTURE_2D, 0);
	// Destroy FreeType once we're finished
	FT_Done_Face(face);
	FT_Done_FreeType(ft);


	// Configure VAO/VBO for texture quads
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

glText::glText(std::string text) : glText()
{
	this->text = text;
}


glText::~glText()
{
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	shader = nullptr;
}

void glText::SetText(std::string text)
{
	this->text = text;
}

std::string glText::GetText()
{
	return text;
}

void glText::SetFontSize(GLfloat size)
{
	fontSize = size;
}

GLfloat glText::GetFontSize()
{
	return fontSize;
}

int glText::GetLength()
{
	return (int)text.size();
}

float glText::GetTextHeight()
{
	float height = 0;
	if (text.length() > 0)
	{
		Character ch = Characters[*text.begin()];
		height = (float)ch.Size.y;
	}
	
	return height;
}

float glText::GetTextWidth()
{
	float Width = 0;
	std::string::const_iterator c;
	for (c = text.begin(); c != text.end(); c++)
	{
		Character ch = Characters[*c];
		Width += ch.Size.x;
	}
	return Width;
}

void glText::Draw(glm::mat4 &VP, double time)
{
	shader->Use();

	GLint timeLocation = glGetUniformLocation(shader->GetProgram(), "Time");
	glUniform1f(timeLocation, (GLfloat)time);

	// Text Drawing!
	glm::vec3 textPos = transform.GetPosition();

	glm::mat4 MVP = VP * transform.WorldMatrix;

	glUniformMatrix4fv(MatrixID, 1, GL_FALSE, glm::value_ptr(MVP));
	glUniform4f(TextColorID, color.R, color.G, color.B, color.A);
	
	GLint sizeLocation = glGetUniformLocation(shader->GetProgram(), "inputSize");
	glUniform1f(sizeLocation, fontSize);
	
	glActiveTexture(GL_TEXTURE0);
	glBindVertexArray(VAO);

	// Iterate through all characters
	std::string::const_iterator c;
	for (c = text.begin(); c != text.end(); c++)
	{
		Character ch = Characters[*c];

		GLfloat xpos = textPos.x + ch.Bearing.x;
		GLfloat ypos = textPos.y - (ch.Size.y - ch.Bearing.y);

		GLfloat w = (GLfloat)ch.Size.x;
		GLfloat h = (GLfloat)ch.Size.y;
		// Update VBO for each character
		GLfloat vertices[6][4] = {
			{ xpos,     ypos + h,   0.0, 0.0 },
			{ xpos,     ypos,       0.0, 1.0 },
			{ xpos + w, ypos,       1.0, 1.0 },

			{ xpos,     ypos + h,   0.0, 0.0 },
			{ xpos + w, ypos,       1.0, 1.0 },
			{ xpos + w, ypos + h,   1.0, 0.0 }
		};
		// Render glyph texture over quad
		glBindTexture(GL_TEXTURE_2D, ch.TextureID);
		// Update content of VBO memory
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices); // Be sure to use glBufferSubData and not glBufferData

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		// Render quad
		glDrawArrays(GL_TRIANGLES, 0, 6);
		// Now advance cursors for next glyph (note that advance is number of 1/64 pixels)
		textPos.x += (ch.Advance >> 6); // Bitshift by 6 to get value in pixels (2^6 = 64 (divide amount of 1/64th pixels by 64 to get amount of pixels))
	}

	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);
}

