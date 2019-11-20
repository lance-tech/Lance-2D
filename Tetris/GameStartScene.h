#pragma once

#include <Engine.h>
#include <Scene/GameScene.h>
#include <Controller/SceneManager.h>

#include <GLObject/glLabel.h>
#include <GLObject/glButton.h>
#include <Core/FileManager.h>

class GameStartScene : public GameScene
{
public:
	GameStartScene();
	~GameStartScene();

	void InitializeGame() override;
	void GameLogic(double &time) override;

private:
	void startGame();
	void endGame();
};

