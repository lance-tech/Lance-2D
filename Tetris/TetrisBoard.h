#pragma once

#include <Engine.h>
#include <vector>

// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <GLObject/Grid.h>
#include <GameObject/TetrisBlock.h>
#include "BlockContainer.h"
#include "BlockBehaviour.h"


class TetrisBoard : public Grid
{
public:
	TetrisBoard();
	TetrisBoard(const unsigned int row, const unsigned int column);
	TetrisBoard(const Object& object);
	~TetrisBoard();
	TetrisBoard& operator=(const TetrisBoard &other);

	void InitializeBoard();

	bool collisionVertical(TetrisBlock& block);
	bool collisionLeftWall(TetrisBlock& block);
	bool collisionRightWall(TetrisBlock& block);
	bool collisionRotate(TetrisBlock& block);
	bool collisionSettleBlcoks(glm::vec3 Pos);

	glm::vec3 GetHitBottomPos(TetrisBlock& tBlock);
	glm::vec3 ConvertBoardCoord(glm::vec3 pos);

	void SetBlock(int row, int column, glRect2D &block);

	bool CheckRowFull(TetrisBlock& block);
	void SettleLogic(TetrisBlock& block);

	bool IsRowFull(int row);

	glRect2D& GetBlock(glRect2D& block);
	void AddBlock(glRect2D& block);

	BlockContainer*** Containers;
	std::vector<glRect2D*> BlocksPool;

	unsigned int BoardRow;
	unsigned int BoardColumn;
};
