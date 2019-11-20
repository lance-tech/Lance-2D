#pragma once
#include <list>
#include <iostream>
#include <GameComponent/GameComponent.h>
#include <Controller/SceneManager.h>
#include <Core/Animator.h>
#include <Scene/GameScene.h>
#include "TetrisBoard.h"


class BoardBehaviour : public GameComponent
{
public:
	BoardBehaviour();
	~BoardBehaviour();
	virtual void Update(double &Time) override;

	void SetSettleBlock(TetrisBlock& block);
	bool CheckBoardFull();
	void UpdateBoardState();
	void SetBlocksAnimation();
	void ClearBoard();
	
	TetrisBoard* GameBoard;
};