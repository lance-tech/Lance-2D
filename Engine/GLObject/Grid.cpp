#include "Grid.h"

Grid::Grid() : glRect2D()
{
	InitializeVertex();
}

Grid::~Grid()
{
}

void Grid::Draw(glm::mat4 &VP, double& time)
{
	glRect2D::Draw(VP, time);

	GLint rowCountLocation = glGetUniformLocation(GetShader()->GetProgram(), "rowCount");
	GLint columnCountLocation = glGetUniformLocation(GetShader()->GetProgram(), "columnCount");
	
	glUniform1i(rowCountLocation, (GLint)transform.GetLocalScale().y);
	glUniform1i(columnCountLocation, (GLint)transform.GetLocalScale().x);
}
