#pragma once
#include <iostream>
#include <GameComponent/GameComponent.h>
#include <GameObject/TetrisBlock.h>


class BlockBehaviour : public GameComponent
{
public:
	BlockBehaviour();
	~BlockBehaviour();

	virtual void Update(double &Time) override;

	void MoveVertical(float y);
	void MoveHorizontal(int direction);
	void Rotate(float degree);

	TetrisBlock* Block;
};

