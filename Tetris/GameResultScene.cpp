#include "GameResultScene.h"


GameResultScene::GameResultScene()
{
	FileManager::OpenScene("Assets/Level/Result Scene.txt", this);

	gameRestartButton = (glButton*)&SceneRoot->FindNode("Restart").GetNodeObject();
	gameRestartButton->Connect(this, &GameResultScene::restartGame);

	gameExitButton = (glButton*)&SceneRoot->FindNode("Exit").GetNodeObject();
	gameExitButton->Connect(this, &GameResultScene::endGame);

	l_myScore = (glLabel*)&SceneRoot->FindNode("MyScore").GetNodeObject();
	l_1st = (glLabel*)&SceneRoot->FindNode("1st").GetNodeObject();
	l_2nd = (glLabel*)&SceneRoot->FindNode("2nd").GetNodeObject();
	l_3rd = (glLabel*)&SceneRoot->FindNode("3rd").GetNodeObject();
}

void GameResultScene::Render(glm::mat4 &VP, double &time)
{
	GameScene::Render(VP, time);
}

GameResultScene::~GameResultScene()
{
}


void GameResultScene::InitializeGame()
{
	GameData& userGameData = GameDataManager::GetUserData();
	std::string scoreStr = std::to_string(userGameData.Score);
	l_myScore->SetText(scoreStr);

	l_1st->SetText(GameDataManager::GetTopScore());
	l_2nd->SetText(GameDataManager::GetTopScore());
	l_3rd->SetText(GameDataManager::GetTopScore());
}

void GameResultScene::GameLogic(double &time)
{
}

void GameResultScene::restartGame()
{
	SceneManager::ResetScene();
	GameDataManager::ResetScores();
}

void GameResultScene::endGame()
{
}