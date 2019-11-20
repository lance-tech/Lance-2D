#pragma once

#include "../Engine.h"
#include "../GLObject/Object.h"
#include "../GLObject/glRect2D.h"


class TetrisBlock : public Object
{
public:
	TetrisBlock();
	~TetrisBlock();
	TetrisBlock(OBJECTTYPE type);

	void Draw(glm::mat4 &VP, double& time) override;

	void SetShader(Shader& shader) override;
	void DestroyShader() override;
	void ReCompileShader(const char *Vfilename = nullptr, const char *Ffilename = nullptr) override;
	virtual const char* GetVertexShader() override;
	virtual const char* GetFragmentShader() override;
	virtual Shader* GetShader() override;
	virtual void SetTexture(char *path) override;

private:
	void initTetrisBlockColor();

public:
	glRect2D *blocks;
	void initTetrisBlockShape();
};

