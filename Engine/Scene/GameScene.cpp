#include "GameScene.h"

GameScene::GameScene()
{
	SceneRoot = new SceneGraph();
}

GameScene::~GameScene()
{
	delete SceneRoot;
}

void GameScene::UpdateTransform()
{
	std::vector<Node*>::iterator it;
	std::vector<Node*>& ChildNodes = SceneRoot->RootNode->GetChildNodes();
	for (it = ChildNodes.begin(); it != ChildNodes.end(); it++)
	{
		Node* adjNode = *it;
		SceneRoot->InheritTransform(*adjNode);
	}
}


void GameScene::UpdateComponents(double& time)
{
	SceneRoot->RootNode->ExecuteOneWay_B(&UpdateObjectComponents, &time);
}

void GameScene::Render(glm::mat4& VP, double& time)
{
	SceneRoot->RootNode->ExecuteOneWay_B(&DrawObject, &VP, &time);
}

void GameScene::OnMousePressed(int Event, double x, double y)
{
	SceneRoot->RootNode->ExecuteOneWay_B(&ObjectPressed, &x, &y);
}