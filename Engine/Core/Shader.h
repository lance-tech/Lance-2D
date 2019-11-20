#ifndef SHADER_H
#define SHADER_H

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include <GL/glew.h>

#include "ImageLoader.h"
#include "Texture.h"

class Shader
{
private:
	unsigned int ID;
	Texture* texture;
	GLuint Program;
	GLuint vertex;
	GLuint fragment;
	std::string VertexShader;
	std::string FragmentShader;
	void AttachShader(GLuint& shader);

public:
	Shader & operator=(const Shader & other);
	const char* GetTexturePath();

	void SetTexture(char* path);
	void ActiveTexture();
	void DisableTexture();

	void ReCompileShader(const GLchar *Vfilename = nullptr, const GLchar *Ffilename = nullptr);
	GLuint CreateShader(const GLchar *vertexPath, int ShaderType);
	
	Shader();
	Shader(const unsigned int id, const GLchar *vertexPath, const GLchar *fragmentPath);
	~Shader();

	const char* GetFragmentShader();
	const char* GetVertexShader();
	GLuint GetProgram();
	unsigned int GetID() const;
	void SetID(const unsigned int id);
	void Use();
};

#endif