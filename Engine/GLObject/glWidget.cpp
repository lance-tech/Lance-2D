#include "glWidget.h"

glWidget::glWidget() : widget(new glRect2D()), text(new glText())
{
}


glWidget::~glWidget()
{
	delete widget;
	delete text;
}

glRect2D* glWidget::GetWidget()
{
	return widget;
}

glText& glWidget::GetTextObject()
{
	return *text;
}

void glWidget::Draw(glm::mat4 &VP, double& time)
{
	widget->transform.SetMatrix(transform.WorldMatrix);
	widget->color = this->color;
	widget->selectColor = this->selectColor;
	widget->Draw(VP, time);

	float fontUnit = text->GetTextWidth() / text->GetLength();
	float textOffsetX = (text->GetLength() / 2.0f) * (fontUnit * text->GetFontSize()) * -1;
	float textOffsetY = (text->GetTextHeight() / 2.0f) * text->GetFontSize() * -1;
	
	glm::vec3 scale = transform.GetLocalScale();
	glm::mat4 scaleMatrix = glm::scale(transform.WorldMatrix, glm::vec3(1.0f / scale.x, 1.0f / scale.y, 1.0f));
	glm::mat4 textMatrix = glm::translate(
		scaleMatrix,
		glm::vec3(textOffsetX, textOffsetY, 0.f));

	text->transform.SetMatrix(textMatrix);

	text->Draw(VP, time);
}


void glWidget::SetText(std::string text)
{
	this->text->SetText(text);
}

void glWidget::SetFontSize(GLfloat size)
{
	this->text->SetFontSize(size);
}
GLfloat glWidget::GetFontSize()
{
	return text->GetFontSize();
}
std::string glWidget::GetText()
{
	return text->GetText();
}

void glWidget::SetTextColor(float r, float g, float b, float a)
{
	text->color.SetColor(r, g, b, a);
}

Color& glWidget::GetTextColor()
{
	return text->color.GetColor();
}


void glWidget::SetShader(Shader& shader)
{
	widget->SetShader(shader);
}

void glWidget::DestroyShader()
{
	widget->DestroyShader();
}

void glWidget::ReCompileShader(const char *Vfilename, const char *Ffilename)
{
	widget->GetShader()->ReCompileShader(Vfilename, Ffilename);
}

const char* glWidget::GetVertexShader()
{
	return widget->GetShader()->GetVertexShader();
}

const char* glWidget::GetFragmentShader()
{
	return widget->GetShader()->GetFragmentShader();
}

Shader* glWidget::GetShader()
{
	return widget->GetShader();
}

void glWidget::SetTexture(char *path)
{
	widget->GetShader()->SetTexture(path);
}
