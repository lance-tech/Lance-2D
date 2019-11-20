#include "glMesh.h"

glMesh::glMesh() : shader(NULL), Object()
{
	Name = "Mesh";
}

glMesh::glMesh(std::string path, std::string vs, std::string fs)
{
	InitializeMesh(path);
}

glMesh::~glMesh()
{
	// Cleanup VBO and shader
	glDeleteVertexArrays(1, &vertexArrayObject);
	glDeleteBuffers(1, &vertexbuffer);
	glDeleteBuffers(1, &uvbuffer);
	glDeleteBuffers(2, &normalbuffer);
	glDeleteBuffers(1, &indexbuffer);
}

void glMesh::InitializeMesh(std::string path)
{
	Path = path;

	vertices.clear();
	uvs.clear();
	normals.clear();
	indices.clear();
	

	glGenVertexArrays(1, &vertexArrayObject);
	glBindVertexArray(vertexArrayObject);


	bool res = ModelLoader::Obj(Path.c_str(), vertices, uvs, normals);

	std::vector<glm::vec3> indexed_vertices;
	std::vector<glm::vec2> indexed_uvs;
	std::vector<glm::vec3> indexed_normals;
	ModelLoader::indexVBO(vertices, uvs, normals, indices, indexed_vertices, indexed_uvs, indexed_normals);

	// Load it into a VBO
	glGenBuffers(1, &vertexbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, indexed_vertices.size() * sizeof(glm::vec3), &indexed_vertices[0], GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glEnableVertexAttribArray(0);


	glGenBuffers(1, &uvbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
	glBufferData(GL_ARRAY_BUFFER, indexed_uvs.size() * sizeof(glm::vec2), &indexed_uvs[0], GL_STATIC_DRAW);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glEnableVertexAttribArray(1);


	glGenBuffers(1, &normalbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, normalbuffer);
	glBufferData(GL_ARRAY_BUFFER, indexed_normals.size() * sizeof(glm::vec3), &indexed_normals[0], GL_STATIC_DRAW);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glEnableVertexAttribArray(2);


	// Generate a buffer for the indices as well
	glGenBuffers(1, &indexbuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexbuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned short), &indices[0], GL_STATIC_DRAW);
	
	glBindVertexArray(0); // Unbind VAO


	// Get a handle for our "LightPosition" uniform
	//glUseProgram(shader->GetProgram());
}


void glMesh::Draw(glm::mat4& VP, double& time)
{
	shader->Use();
	shader->ActiveTexture();

	LightID = glGetUniformLocation(shader->GetProgram(), "LightPosition_worldspace");
	MatrixID = glGetUniformLocation(shader->GetProgram(), "MVP");
	ViewMatrixID = glGetUniformLocation(shader->GetProgram(), "V");
	ModelMatrixID = glGetUniformLocation(shader->GetProgram(), "M");
	ProjectionMatrixID = glGetUniformLocation(shader->GetProgram(), "P");

	GLint timeLocation = glGetUniformLocation(shader->GetProgram(), "Time");
	glUniform1f(timeLocation, (GLfloat)time);

	// Setup Camera
	Projection = glm::ortho(
		WINDOW_LEFT,
		WINDOW_RIGHT,
		WINDOW_BOTTOM,
		WINDOW_TOP,
		-100.0f, 100.0f);
	View = glm::lookAt(
		glm::vec3(0, 0, 1), 
		glm::vec3(0, 0, 0),
		glm::vec3(0, 1, 0)
	);

	glm::mat4 MVP = VP * transform.WorldMatrix;

	// Send our transformation to the currently bound shader, 
	glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);
	glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &transform.WorldMatrix[0][0]);
	glUniformMatrix4fv(ViewMatrixID, 1, GL_FALSE, &View[0][0]);
	glUniformMatrix4fv(ProjectionMatrixID, 1, GL_FALSE, &Projection[0][0]);

	glm::vec3 lightPos = glm::vec3(100, 100, 100);
	glUniform3f(LightID, lightPos.x, lightPos.y, lightPos.z);

	GLint viewPosID = glGetUniformLocation(shader->GetProgram(), "ViewPos");
	glUniform3f(viewPosID, 0.0f, 0.0f, 50.0f);

	GLint worldModelPosID = glGetUniformLocation(shader->GetProgram(), "WorldModelPos");
	glm::vec3 wp = transform.GetPosition();
	glUniform3f(worldModelPosID, wp.x, wp.y, wp.z);
	

	GLint specularFactorLocation = glGetUniformLocation(shader->GetProgram(), "specularFactor");
	glUniform1f(specularFactorLocation, color.Specular);


	GLint colorLocation = glGetUniformLocation(shader->GetProgram(), "inputColor");
	Color finalColor(color.R + selectColor.R,
		color.G + selectColor.G,
		color.B + selectColor.B);
	glUniform4f(colorLocation, finalColor.R, finalColor.G, finalColor.B, color.A);

	glBindVertexArray(vertexArrayObject);
	glDrawElements(GL_TRIANGLES, (GLsizei)indices.size(), GL_UNSIGNED_SHORT, (void*)0);
	glBindVertexArray(0);

	//glBindTexture(GL_TEXTURE_2D, 0);
	
	shader->DisableTexture();
}

std::string glMesh::GetMeshPath()
{
	return Path;
}

void glMesh::SetShader(Shader& shader)
{
	this->shader = &shader;
}

void glMesh::DestroyShader()
{
	shader = nullptr;
}

void glMesh::ReCompileShader(const char *Vfilename, const char *Ffilename)
{
	shader->ReCompileShader(Vfilename, Ffilename);
}

const char* glMesh::GetVertexShader()
{
	return shader->GetVertexShader();
}

const char* glMesh::GetFragmentShader()
{
	return shader->GetFragmentShader();
}

Shader* glMesh::GetShader()
{
	return shader;
}

void glMesh::SetTexture(char *path)
{
	shader->SetTexture(path);
}

