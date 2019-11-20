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
#include "../Core/Shader.h"
#include "../Core/ModelLoader.h"
#include "Object.h"

class glMesh : public Object
{
public:
	glMesh();
	glMesh(std::string path, std::string vs, std::string fs);
	virtual ~glMesh();

	void InitializeMesh(std::string path);
	void Draw(glm::mat4 &VP, double& time) override;

	Shader* shader;
	void SetShader(Shader& shader) override;
	void DestroyShader() override;
	void ReCompileShader(const char *Vfilename = nullptr, const char *Ffilename = nullptr) override;
	virtual const char* GetVertexShader() override;
	virtual const char* GetFragmentShader() override;
	virtual Shader* GetShader() override;
	virtual void SetTexture(char *path) override;

	std::string GetMeshPath();

private:
	std::string Path;

	GLuint MatrixID;
	GLuint ViewMatrixID;
	GLuint ModelMatrixID;
	GLuint ProjectionMatrixID;
	GLuint LightID;

	glm::mat4 Projection;
	glm::mat4 View;
	glm::mat4 Model;
	glm::mat4 MVP;

	std::vector<glm::vec3> vertices;
	std::vector<glm::vec2> uvs;
	std::vector<glm::vec3> normals;
	std::vector<unsigned short> indices;

	GLuint vertexbuffer;
	GLuint uvbuffer;
	GLuint normalbuffer;
	GLuint indexbuffer;
	GLuint vertexArrayObject;
};
