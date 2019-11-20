#include "Shader.h"


Shader & Shader::operator=(const Shader & other)
{
	this->Program = other.Program;
	this->VertexShader = other.VertexShader;
	this->FragmentShader = other.FragmentShader;
	this->texture = other.texture;
	this->ReCompileShader();
	return *this;
}


const char* Shader::GetTexturePath()
{
	if (texture)
	{
		return texture->Path.c_str();
	}
	return "";
}

void Shader::SetTexture(char* path)
{
	char* res = strstr(path, ".DDS");
	if (res)
	{
		texture = new Texture();
		texture->ID = ImageLoader::DDS(path);
		texture->Path = path;
		texture->SamplerID = glGetUniformLocation(Program, "TextureSampler0");
	}
	res = strstr(path, ".dds");
	if (res)
	{
		texture = new Texture();
		texture->ID = ImageLoader::DDS(path);
		texture->Path = path;
		texture->SamplerID = glGetUniformLocation(Program, "TextureSampler0");
	}
	res = strstr(path, ".BMP");
	if (res)
	{
		texture = new Texture();
		texture->ID = ImageLoader::BMP(path);
		texture->Path = path;
		texture->SamplerID = glGetUniformLocation(Program, "TextureSampler0");
	}
	res = strstr(path, ".bmp");
	if (res)
	{
		texture = new Texture();
		texture->ID = ImageLoader::BMP(path);
		texture->Path = path;
		texture->SamplerID = glGetUniformLocation(Program, "TextureSampler0");
	}
}

void Shader::ActiveTexture()
{
	if (texture)
	{
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture->ID);
		glUniform1i(texture->SamplerID, 0);
	}
}

void Shader::DisableTexture()
{
	if (texture)
	{
		glBindTexture(GL_TEXTURE_2D, 0);
	}
}

void Shader::ReCompileShader(const GLchar *Vfilename, const GLchar *Ffilename)
{
	glDetachShader(this->Program, GL_VERTEX_SHADER);
	glDetachShader(this->Program, GL_FRAGMENT_SHADER);
	glDeleteProgram(this->Program);

	this->Program = glCreateProgram();

	if (Vfilename)
	{
		VertexShader = Vfilename;
		vertex = CreateShader(Vfilename, GL_VERTEX_SHADER);
		AttachShader(vertex);
	}
	else
	{
		vertex = CreateShader(VertexShader.c_str(), GL_VERTEX_SHADER);
		AttachShader(vertex);
	}

	if (Ffilename)
	{
		FragmentShader = Ffilename;
		fragment = CreateShader(Ffilename, GL_FRAGMENT_SHADER);
		AttachShader(fragment);
	}
	else
	{
		fragment = CreateShader(FragmentShader.c_str(), GL_FRAGMENT_SHADER);
		AttachShader(fragment);
	}
}

GLuint Shader::CreateShader(const GLchar *shaderPath, int ShaderType)
{
	std::string shaderStr;
	std::ifstream ShaderFile;
	ShaderFile.exceptions(std::ifstream::badbit);
	try
	{
		// Open files
		ShaderFile.open(shaderPath);
		std::stringstream ShaderStream;
		// Read file's buffer contents into streams
		ShaderStream << ShaderFile.rdbuf();
		// close file handlers
		ShaderFile.close();
		// Convert stream into string
		shaderStr = ShaderStream.str();
	}
	catch (std::ifstream::failure e)
	{
		std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
	}
	const GLchar *ShaderCode = shaderStr.c_str();

	GLint success;
	GLchar infoLog[512];
	// Vertex Shader
	GLuint shader = glCreateShader(ShaderType);
	glShaderSource(shader, 1, &ShaderCode, NULL);
	glCompileShader(shader);
	// Print compile errors if any
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertex, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	return shader;
}

void Shader::AttachShader(GLuint& shader)
{
	GLint success;
	GLchar infoLog[512];

	// Shader Program
	glAttachShader(this->Program, shader);
	glLinkProgram(this->Program);
	// Print linking errors if any
	glGetProgramiv(this->Program, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(this->Program, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}
	glDeleteShader(shader);
}

Shader::Shader() : 
	ID(0), texture(nullptr), Program(glCreateProgram()),
	vertex(), fragment(),
	VertexShader(), FragmentShader()
{
}

Shader::Shader(const unsigned int id, const GLchar *vertexPath, const GLchar *fragmentPath) : Shader()
{
	ID = id;
	VertexShader = vertexPath;
	FragmentShader = fragmentPath;

	vertex = CreateShader(vertexPath, GL_VERTEX_SHADER);
	fragment = CreateShader(fragmentPath, GL_FRAGMENT_SHADER);
	AttachShader(vertex);
	AttachShader(fragment);
}

Shader::~Shader()
{
	if (texture)
	{
		delete texture;
	}
	glDeleteProgram(Program);
}

const char* Shader::GetFragmentShader()
{
	return FragmentShader.c_str();
}

const char* Shader::GetVertexShader()
{
	return VertexShader.c_str();
}

GLuint Shader::GetProgram()
{
	return Program;
}

unsigned int Shader::GetID() const
{
	return ID;
}

void Shader::SetID(const unsigned int id)
{
	ID = id;
}

void Shader::Use()
{
	glUseProgram(Program);
}