#include "GameWindow.h"

GameWindow::GameWindow() : glWindow(0, 0)
{
	Initialize();
}

GameWindow::~GameWindow()
{
}

void GameWindow::Initialize()
{
	GameStartScene *sceneGameStart = new GameStartScene();
	SceneManager::AddGameScene(*sceneGameStart);

	GameMainScene *sceneGameMain = new GameMainScene();
	SceneManager::AddGameScene(*sceneGameMain);

	GameResultScene *sceneGameResult = new GameResultScene();
	SceneManager::AddGameScene(*sceneGameResult);

	SceneManager::ResetScene();
}

void GameWindow::Update(double &deltaTime)
{
	glWindow::Update(deltaTime);
}