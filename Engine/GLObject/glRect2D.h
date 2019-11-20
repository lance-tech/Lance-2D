#pragma once

#include "../Engine.h"

// OpenGL
#include <GL/glew.h>

// GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/euler_angles.hpp>

// Other
#include <cmath>
#include "../Core/Shader.h"
#include "Object.h"

class glRect2D : public Object
{
public:
	glRect2D();
	virtual ~glRect2D();
	glRect2D& operator=(const glRect2D &other);

	void InitializeVertex();

	void Draw(glm::mat4 &VP, double& time) override;

	void SetShader(Shader& shader) override;
	void DestroyShader() override;
	void ReCompileShader(const char *Vfilename = nullptr, const char *Ffilename = nullptr) override;
	virtual const char* GetVertexShader() override;
	virtual const char* GetFragmentShader() override;
	virtual Shader* GetShader() override;
	virtual void SetTexture(char *path) override;

private:
	Shader* shader;
	GLfloat color_data[12];
	GLfloat vertex_data[12];
	GLfloat uv_data[8];
	GLuint index_data[6];
	
	GLuint vertexbuffer;
	GLuint uvbuffer;
	GLuint indexbuffer;
	GLuint colorbuffer;
	GLuint vertexArrayObject;
};