#include "EmptyScene.h"


EmptyScene::EmptyScene() : GameScene()
{
	gridBoard = (Grid*)(&SceneManager::CreateObject(OBJECTTYPE::GAMEBOARD));
	Shader& shader = MaterialManager::CreateShader("Assets/shaders/Grid.vert", "Assets/shaders/Grid.frag");
	gridBoard->SetShader(shader);
	gridBoard->IsSelected = false;
	gridBoard->transform.SetWorldPosition(0.f, 0.f, -90.0f);
	gridBoard->transform.SetWorldScale(40.f, 40.f, 1.0f);
	gridBoard->color.SetColor(0.35f, 0.35f, 0.35f);
}


EmptyScene::~EmptyScene()
{
	delete gridBoard;
}

void EmptyScene::InitializeGame()
{
}

void EmptyScene::GameLogic(double &time)
{
}

void EmptyScene::Render(glm::mat4 &VP, double &time)
{
	gridBoard->Draw(VP, time);
	GameScene::Render(VP, time);
}