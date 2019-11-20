#pragma once

// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/euler_angles.hpp>

#include "../Engine.h"
#include "../Core/TimeLineModel.h"
#include "../Controller/SceneGraph.h"
#include "../GameComponent/GameComponent.h"

class GameScene
{
public:
	GameScene();
	virtual ~GameScene();

	virtual void InitializeGame() = 0;
	virtual void GameLogic(double &time) = 0;
	virtual void Render(glm::mat4 &VP, double &time);

	void UpdateTransform();
	void UpdateComponents(double& time);

	SceneGraph* SceneRoot;

	virtual void OnKeyPressed(int Event) {};
	virtual void OnKeyReleased(int Event) {}
	virtual void OnMousePressed(int Event, double x, double y);
	virtual void OnMouseMove(double x, double y) {}
	virtual void OnMouseReleased(int Event, double x, double y) {}

	TimeLineModel timeline;
};