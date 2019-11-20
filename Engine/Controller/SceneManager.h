#pragma once
#include <vector>
#include "AnimationManager.h"
#include "MaterialManager.h"
#include "../Scene/GameScene.h"
#include "../GLObject/Object.h"
#include "../GLObject/glMesh.h"
#include "../GLObject/glButton.h"
#include "../GLObject/glLabel.h"
#include "../GLObject/Grid.h"
#include "../GameObject/TetrisBlock.h"


class SceneManager
{
public:
	SceneManager();
	~SceneManager();

	void SetTickTime(double tickCount);
	static double GetTickTime();

	static void AddGameScene(GameScene &gameScene);
	static void updateCurrentScene(glm::mat4 &VP, double& Time);
	static void SetNextScene();
	static void ResetScene();
	
	static GameScene* GetCurrentScene();
	static SceneManager* GetInstance();
	static Object& CreateObject(OBJECTTYPE wType, const char* v_path = nullptr, const char* f_path = nullptr, bool refShader = true);
	static Object& CopyObject(const Object& sourceObject);

	static void SetRootNode(Node& rootNode);
	static Node& GetRootNode();
	static SceneGraph& GetSceneRoot();

	static AnimationManager animationManager;

private:
	std::vector<GameScene*> scenes;
	double Time;
	static bool Instantiated;
	int currentIndex;
	static SceneManager* Instance;

	void DoItFirst();
};
