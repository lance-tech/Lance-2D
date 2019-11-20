#pragma once

#include <string>
#include <Scene/GameScene.h>
#include <Controller/SceneManager.h>
#include <Controller/GameDataManager.h>
#include <Core/FileManager.h>

#include <GLObject/glLabel.h>
#include <GLObject/glButton.h>
#include <GLObject/glRect2D.h>
#include <GLObject/glMesh.h>


class GameResultScene : public GameScene
{
public:
	GameResultScene();
	~GameResultScene();

	void InitializeGame() override;
	void GameLogic(double &time) override;
	void Render(glm::mat4 &VP, double &time) override;

private:
	glLabel *l_myScore;
	glLabel *l_1st;
	glLabel *l_2nd;
	glLabel *l_3rd;
	glButton *gameRestartButton;
	glButton *gameExitButton;

	void restartGame();
	void endGame();
};

