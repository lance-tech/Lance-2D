#pragma once

#include <string.h>
#include <vector>
#include <deque>

#include <Scene/GameScene.h>
#include <GLObject/glLabel.h>
#include <GLObject/glButton.h>

#include <Controller/SceneGraph.h>
#include <Controller/SceneManager.h>
#include <Controller/GameDataManager.h>
#include <Core/FileManager.h>

#include "TetrisBoard.h"
#include <GameObject/TetrisBlock.h>

#include <Core/Animator.h>
#include "BoardBehaviour.h"
#include "BlockBehaviour.h"

#include <Core/Control.h>

class GameMainScene : public GameScene
{
public:
	GameMainScene();
	~GameMainScene();

	TetrisBlock* CreateTetrisBlock();

	void InitializeGame() override;
	void GameLogic(double &time) override;
	void Render(glm::mat4 &VP, double &time) override;

	void DownStraight();
	void MoveVertical(GLfloat y);
	void MoveHorizontal(int direction);
	void Rotate();

protected:
	virtual void OnKeyPressed(int Event) override;

private:
	std::deque<TetrisBlock*> BlockQueue;

	void IncreaseScore();
	bool CheckLevel(double time);
	void DestroyGame();
	void GameOver();

	BoardBehaviour *boardBehaviour;
	BlockBehaviour* blockBehaviour;
	TetrisBlock *ActiveBlock;
	TetrisBlock *guideTBlock;
	TetrisBlock *ghostTBlock;
	TetrisBoard *GameBoard;
	glLabel *l_score;
	glLabel *l_level;
	Node *queueNode[3];
	int level;
	unsigned int score;
	double lastTime;
	double timeAccum;
	double interval;
};