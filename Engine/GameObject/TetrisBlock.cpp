#include "TetrisBlock.h"

TetrisBlock::TetrisBlock() : blocks(nullptr)
{
	Type = OBJECTTYPE::block0;
	blocks = new glRect2D[4];
}

TetrisBlock::TetrisBlock(OBJECTTYPE type) : TetrisBlock()
{
	Type = type;
	
	initTetrisBlockColor();
	initTetrisBlockShape();
}

TetrisBlock::~TetrisBlock()
{
	delete[] blocks;
}

void TetrisBlock::initTetrisBlockColor()
{
	switch (Type) {
	case OBJECTTYPE::block1:
	{
		color.SetColor(1.0f, 0.05f, 0.0f, 1.0f);
		break;
	}
	case OBJECTTYPE::block2:
	{
		color.SetColor(0.05f, 1.0f, 0.1f, 1.0f);
		break;
	}
	case OBJECTTYPE::block3:
	{
		color.SetColor(0.1f, 0.1f, 0.9f, 1.0f);
		break;
	}
	case OBJECTTYPE::block4:
	{
		color.SetColor(0.1f, 1.0f, 0.2f, 1.0f);
		break;
	}
	case OBJECTTYPE::block5:
	{
		color.SetColor(0.1f, 0.8f, 0.8f, 1.0f);
		break;
	}
	case OBJECTTYPE::block6:
	{
		color.SetColor(0.8f, 0.2f, 0.8f, 1.0f);
		break;
	}
	case OBJECTTYPE::block7:
	{
		color.SetColor(0.7f, 0.5f, 0.2f, 1.0f);
		break;
	}
	default:
	{
		blocks[0].color.SetColor(1.0, 0.0, 0.0, 1.0);
		blocks[1].color.SetColor(0.0, 1.0, 0.0, 1.0);
		blocks[2].color.SetColor(1.0, 1.0, 0.0, 1.0);
		blocks[3].color.SetColor(0.0, 0.0, 1.0, 1.0);
		break;
	}
	}
}

void TetrisBlock::initTetrisBlockShape()
{
	blocks[0].transform.SetMatrix(transform.WorldMatrix);
	blocks[1].transform.SetMatrix(transform.WorldMatrix);
	blocks[2].transform.SetMatrix(transform.WorldMatrix);
	blocks[3].transform.SetMatrix(transform.WorldMatrix);

	switch (Type) {
	case OBJECTTYPE::block1:
	{
		blocks[0].transform.SetTranslate(-1.0f, -1.0f, 0.0f);
		blocks[1].transform.SetTranslate(-1.0f, 0.0f, 0.0f);
		blocks[2].transform.SetTranslate(-1.0f, 1.0f, 0.0f);
		blocks[3].transform.SetTranslate(0.0f, 1.0f, 0.0f);
		break;
	}
	case OBJECTTYPE::block2:
	{
		blocks[0].transform.SetTranslate(1.0f, -1.0f, 0.0f);
		blocks[1].transform.SetTranslate(1.0f, 0.0f, 0.0f);
		blocks[2].transform.SetTranslate(1.0f, 1.0f, 0.0f);
		blocks[3].transform.SetTranslate(0.0f, 1.0f, 0.0f);
		break;
	}
	case OBJECTTYPE::block3:
	{
		blocks[0].transform.SetTranslate(-1.0f, 0.0f, 0.0f);
		blocks[1].transform.SetTranslate(0.0f, 0.0f, 0.0f);
		blocks[2].transform.SetTranslate(1.0f, 0.0f, 0.0f);
		blocks[3].transform.SetTranslate(0.0f, 1.0f, 0.0f);
		break;
	}
	case OBJECTTYPE::block4:
	{
		blocks[0].transform.SetTranslate(-1.0f, 0.0f, 0.0f);
		blocks[1].transform.SetTranslate(0.0f, 0.0f, 0.0f);
		blocks[2].transform.SetTranslate(0.0f, 1.0f, 0.0f);
		blocks[3].transform.SetTranslate(1.0f, 1.0f, 0.0f);
		break;
	}
	case OBJECTTYPE::block5:
	{
		blocks[0].transform.SetTranslate(-1.0f, 1.0f, 0.0f);
		blocks[1].transform.SetTranslate(0.0f, 1.0f, 0.0f);
		blocks[2].transform.SetTranslate(0.0f, 0.0f, 0.0f);
		blocks[3].transform.SetTranslate(1.0f, 0.0f, 0.0f);
		break;
	}
	case OBJECTTYPE::block6:
	{
		blocks[0].transform.SetTranslate(0.0f, 0.0f, 0.0f);
		blocks[1].transform.SetTranslate(1.0f, 0.0f, 0.0f);
		blocks[2].transform.SetTranslate(0.0f, 1.0f, 0.0f);
		blocks[3].transform.SetTranslate(1.0f, 1.0f, 0.0f);
		break;
	}
	case OBJECTTYPE::block7:
	{
		blocks[0].transform.SetTranslate(0.0f, -1.0f, 0.0f);
		blocks[1].transform.SetTranslate(0.0f, 0.0f, 0.0f);
		blocks[2].transform.SetTranslate(0.0f, 1.0f, 0.0f);
		blocks[3].transform.SetTranslate(0.0f, 2.0f, 0.0f);
		break;
	}
	default:
	{
		blocks[0].transform.SetTranslate(0.0f, 0.0f, 0.0f);
		blocks[1].transform.SetTranslate(0.0f, 1.0f, 0.0f);
		blocks[2].transform.SetTranslate(0.0f, 2.0f, 0.0f);
		blocks[3].transform.SetTranslate(0.0f, 3.0f, 0.0f);
		break;
	}
	}
} 

void TetrisBlock::Draw(glm::mat4 &VP, double& time)
{
	initTetrisBlockShape();

	for (int i = 0; i < 4; i++)
	{
		blocks[i].color = this->color;
		blocks[i].selectColor = this->selectColor;
		blocks[i].Draw(VP, time);
	}
}

void TetrisBlock::SetShader(Shader& shader)
{
	for (int i = 0; i < 4; i++)
	{
		blocks[i].SetShader(shader);
	}
}

void TetrisBlock::DestroyShader()
{
	for (int i = 0; i < 4; i++)
	{
		blocks[i].DestroyShader();
	}
}

void TetrisBlock::ReCompileShader(const char *Vfilename, const char *Ffilename)
{
	for (int i = 0; i < 4; i++)
	{
		blocks[i].GetShader()->ReCompileShader(Vfilename, Ffilename);
	}
}

const char* TetrisBlock::GetVertexShader()
{
	return blocks[0].GetShader()->GetVertexShader();
}

const char* TetrisBlock::GetFragmentShader()
{
	return blocks[0].GetShader()->GetFragmentShader();
}

Shader* TetrisBlock::GetShader()
{
	return blocks[0].GetShader();
}

void TetrisBlock::SetTexture(char *path)
{
	for (int i = 0; i < 4; i++)
	{
		blocks[i].GetShader()->SetTexture(path);
	}
}