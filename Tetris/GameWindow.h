#pragma once
#include <iostream>
#include <Core/Control.h>
#include <GLObject/glWindow.h>
#include "GameStartScene.h"
#include "GameMainScene.h"
#include "GameResultScene.h"

class GameWindow : public glWindow
{
public:
	GameWindow();
	~GameWindow();

protected:
	void Initialize() override;
	void Update(double &deltaTime) override;
};

