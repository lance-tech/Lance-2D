#pragma once

#include "../Engine.h"
#include "glRect2D.h"
#include "Object.h"
#include "glText.h"

class glWidget : public Object
{
public:
	glWidget();
	virtual ~glWidget();

	void Draw(glm::mat4 &VP, double& time) override;

	void SetShader(Shader& shader) override;
	void DestroyShader() override;
	void ReCompileShader(const char *Vfilename = nullptr, const char *Ffilename = nullptr) override;
	virtual const char* GetVertexShader() override;
	virtual const char* GetFragmentShader() override;
	virtual Shader* GetShader() override;
	virtual void SetTexture(char *path) override;

protected:
	glRect2D *widget;
	glText *text;

public:
	glRect2D* GetWidget();
	glText& GetTextObject();

	void SetText(std::string text);
	void SetFontSize(GLfloat size);
	GLfloat GetFontSize();
	std::string GetText();
	void SetTextColor(float r, float g, float b, float a = 1.0f);
	Color& GetTextColor();
};