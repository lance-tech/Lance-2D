#include "GameStartScene.h"


GameStartScene::GameStartScene() : GameScene()
{
	FileManager::OpenScene("Assets/Level/Start Scene.txt", this);

	glButton &gameStartButton = (glButton&)SceneRoot->FindNode("StartButton").GetNodeObject();
	gameStartButton.Connect(this, &GameStartScene::startGame);
	
	glButton &gameExitButton = (glButton&)SceneRoot->FindNode("ExitButton").GetNodeObject();
	gameExitButton.Connect(this, &GameStartScene::endGame);
}


GameStartScene::~GameStartScene()
{
}


void GameStartScene::startGame()
{
	SceneManager::SetNextScene();
}

void GameStartScene::endGame()
{
	std::cout << "Pressed EndGame()" << std::endl;
}

void GameStartScene::InitializeGame()
{
}

void GameStartScene::GameLogic(double &time)
{
}
