#include "glRect2D.h"

glRect2D::glRect2D() : shader(nullptr)
{
	InitializeVertex();
}

glRect2D& glRect2D::operator=(const glRect2D &other)
{
	this->shader = other.shader;
	this->vertexbuffer = other.vertexbuffer;
	this->uvbuffer = other.uvbuffer;
	this->indexbuffer = other.indexbuffer;
	this->colorbuffer = other.colorbuffer;
	this->vertexArrayObject = other.vertexArrayObject;
	for (int i = 0; i < 10; i++)
	{
		this->vertex_data[i] = other.vertex_data[i];
		this->color_data[i] = other.color_data[i];
	}
	for (int i = 0; i < 8; i++)
	{
		this->uv_data[i] = other.uv_data[i];
	}
	for (int i = 0; i < 6; i++)
	{
		this->index_data[i] = other.index_data[i];
	}
	this->InitializeVertex();
	return *this;
}

glRect2D::~glRect2D()
{
	glDeleteVertexArrays(1, &vertexArrayObject);
	glDeleteBuffers(1, &vertexbuffer);
	glDeleteBuffers(1, &indexbuffer);
	glDeleteBuffers(1, &colorbuffer);
	glDeleteBuffers(1, &uvbuffer);
}

void glRect2D::SetShader(Shader& shader)
{
	this->shader = &shader;
}

void glRect2D::DestroyShader()
{
	this->shader = nullptr;
}

void glRect2D::ReCompileShader(const char *Vfilename, const char *Ffilename)
{
	shader->ReCompileShader(Vfilename, Ffilename);
}

const char* glRect2D::GetVertexShader()
{
	return shader->GetVertexShader();
}

const char* glRect2D::GetFragmentShader()
{
	return shader->GetFragmentShader();
}

Shader* glRect2D::GetShader()
{
	return shader;
}

void glRect2D::SetTexture(char *path)
{
	shader->SetTexture(path);
}


void glRect2D::InitializeVertex()
{
	// Initialize Shader Program
	glGenVertexArrays(1, &vertexArrayObject);
	glBindVertexArray(vertexArrayObject);

	color_data[0] = 1.0f;
	color_data[1] = 1.0f;
	color_data[2] = 1.0f;
	color_data[3] = 1.0f;
	color_data[4] = 1.0f;
	color_data[5] = 1.0f;
	color_data[6] = 1.0f;
	color_data[7] = 1.0f;
	color_data[8] = 1.0f;
	color_data[9] = 1.0f;
	color_data[10] = 1.0f;
	color_data[11] = 1.0f;

	//// CW
	//index_data[0] = 0;
	//index_data[1] = 1;
	//index_data[2] = 3;
	//index_data[3] = 1;
	//index_data[4] = 2;
	//index_data[5] = 3;

	// CCW
	index_data[0] = 0;
	index_data[1] = 3;
	index_data[2] = 1;
	index_data[3] = 1;
	index_data[4] = 3;
	index_data[5] = 2;

	uv_data[0] = 1.0f;
	uv_data[1] = 1.0f;
	uv_data[2] = 1.0f;
	uv_data[3] = 0.0f;
	uv_data[4] = 0.0f;
	uv_data[5] = 0.0f;
	uv_data[6] = 0.0f;
	uv_data[7] = 1.0f;

	glm::vec3 scale = transform.GetLocalScale();
	GLfloat left = (scale.x / 2.f) * -1.f;
	GLfloat right = scale.x / 2.f;
	GLfloat top = scale.y / 2.f;
	GLfloat bottom = (scale.y / 2.f) * -1.f;

	vertex_data[0] = right;
	vertex_data[1] = top;
	vertex_data[2] = transform.Z;
	vertex_data[3] = right;
	vertex_data[4] = bottom;
	vertex_data[5] = transform.Z;
	vertex_data[6] = left;
	vertex_data[7] = bottom;
	vertex_data[8] = transform.Z;
	vertex_data[9] = left;
	vertex_data[10] = top;
	vertex_data[11] = transform.Z;

	

	glGenBuffers(1, &vertexbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_data), vertex_data, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0);
	glEnableVertexAttribArray(0);


	glGenBuffers(1, &colorbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(color_data), color_data, GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0);
	glEnableVertexAttribArray(1);

	glGenBuffers(1, &uvbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(uv_data), uv_data, GL_STATIC_DRAW);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0);
	glEnableVertexAttribArray(2);


	glGenBuffers(1, &indexbuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexbuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(index_data), index_data, GL_STATIC_DRAW);


	glBindVertexArray(0); // Unbind VAO
}


void glRect2D::Draw(glm::mat4& VP, double& time)
{
	//glClear(GL_DEPTH_BUFFER_BIT);
	//glEnable(GL_BLEND);
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	shader->Use();
	shader->ActiveTexture();

	GLint timeLocation = glGetUniformLocation(shader->GetProgram(), "Time");
	glUniform1f(timeLocation, (GLfloat)time);
	
	
	GLint colorLocation = glGetUniformLocation(shader->GetProgram(), "inputColor");

	Color finalColor(color.R + selectColor.R,
					color.G + selectColor.G,
					color.B + selectColor.B);

	glUniform4f(colorLocation, finalColor.R, finalColor.G, finalColor.B, color.A);
	
	glm::mat4 MVP = VP * transform.WorldMatrix;
	
	GLint transformLocation = glGetUniformLocation(shader->GetProgram(), "MVP");
	glUniformMatrix4fv(transformLocation, 1, GL_FALSE, glm::value_ptr(MVP));


	GLint specularFactorLocation = glGetUniformLocation(shader->GetProgram(), "specularFactor");
	glUniform1f(specularFactorLocation, color.Specular);



	glBindVertexArray(vertexArrayObject);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

	shader->DisableTexture();
}