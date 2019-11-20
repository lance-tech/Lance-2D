#pragma once
#include "../Engine.h"
#include "GameScene.h"
#include "../Controller/SceneManager.h"
#include "../Controller/MaterialManager.h"
#include "../GLObject/Grid.h"
#include "../GLObject/glRect2D.h"


class EmptyScene : public GameScene
{
public:
	EmptyScene();
	virtual ~EmptyScene() override;

	void InitializeGame() override;
	void GameLogic(double &time) override;
	void Render(glm::mat4 &VP, double &time) override;

private:
	Grid* gridBoard;
};
