#include "TetrisBoard.h"
#include "BoardBehaviour.h"


TetrisBoard::TetrisBoard() : BoardRow(0), BoardColumn(0)
{
	InitializeBoard();
}

TetrisBoard::TetrisBoard(const unsigned int row, const unsigned int column)
	: BoardRow(row), BoardColumn(column)
{
	InitializeBoard();
}

TetrisBoard::TetrisBoard(const Object& object)
	: TetrisBoard((unsigned int)object.transform.SY, (unsigned int)object.transform.SX)
{
	this->IsSelected = object.IsSelected;
	this->Type = object.Type;
	this->transform = object.transform;
	this->color = object.color;
	this->selectColor = object.selectColor;
	this->animation = object.animation;
	this->Name = object.Name;
}

TetrisBoard& TetrisBoard::operator=(const TetrisBoard &other)
{
	for (unsigned int row = 0; row < BoardRow; row++)
	{
		for (unsigned int col = 0; col < BoardColumn; col++)
		{
			this->Containers[row][col] = other.Containers[row][col];
		}
	}
	
	this->Name = other.Name;
	return *this;
}

void TetrisBoard::InitializeBoard()
{
	this->Containers = new BlockContainer **[BoardRow];
	for (unsigned int row = 0; row < BoardRow; row++)
	{
		this->Containers[row] = new BlockContainer *[BoardColumn];
		for (unsigned int col = 0; col < BoardColumn; col++)
		{
			this->Containers[row][col] = new BlockContainer();
		}
	}
}

TetrisBoard::~TetrisBoard()
{
	for (unsigned int row = 0; row < BoardRow; row++)
	{
		delete[] this->Containers[row];
	}
	delete[] this->Containers;
}

bool TetrisBoard::CheckRowFull(TetrisBlock& block)
{
	glm::vec3 boardPos = transform.GetPositionSnap();
	glm::vec3 boardScale = transform.GetLocalScale();
	float top = boardScale.y / 2.f;
	float boardTop = boardPos.y + top;
	for (int i = 0; i < 4; i++)
	{
		glm::vec3 Pos = block.blocks[i].transform.GetPositionSnap();

		if (Pos.y >= boardTop) {
			return true;
		}
	}
	return false;
}

void TetrisBoard::SettleLogic(TetrisBlock& block)
{
	BoardBehaviour* boardBehaviour = (BoardBehaviour*)GetComponent("BoardBehaviour");

	// Settle Block that has settled on bottom
	boardBehaviour->SetSettleBlock(block);

	// Check rows is full or not
	// Mark up board state for need to delete block coordinate in board.
	if (boardBehaviour->CheckBoardFull())
	{
		// Blocks has contained setup animation
		boardBehaviour->SetBlocksAnimation();

		// Re mark up board state after animation
		boardBehaviour->UpdateBoardState();

		// animation start
		SceneManager::animationManager.PlayAnimators();
	}
	
	block.Name = "Settle Block";
	BlockBehaviour* blockBehaviour = (BlockBehaviour*)block.GetComponent("BlockBehaviour");
	block.RemoveComponent(blockBehaviour);

	Node& settleBlockNode = SceneManager::GetSceneRoot().FindNode("Settle Block");
	settleBlockNode.DestroyNode(settleBlockNode);
}

glm::vec3 TetrisBoard::ConvertBoardCoord(glm::vec3 pos)
{
	glm::vec3 ConvertedPos;
	glm::vec3 boardPos = transform.GetPositionSnap();
	glm::vec3 boardScale = transform.GetLocalScale();

	ConvertedPos.x = pos.x - boardPos.x + (boardScale.x / 2);
	ConvertedPos.y = pos.y - boardPos.y + (boardScale.y / 2);

	return ConvertedPos;
}

void TetrisBoard::SetBlock(int row, int column, glRect2D &block)
{
	Containers[row][column]->SetSettlBlock(block);
}

bool TetrisBoard::IsRowFull(int row)
{
	bool res = true;
	for (unsigned int column = 0; column < BoardColumn; column++)
	{
		if (Containers[row][column]->GetEmpty())
		{
			res = false;
		}
	}
	return res;
}


bool TetrisBoard::collisionSettleBlcoks(glm::vec3 Pos)
{
	bool res = false;

	glm::vec3 boardPos = transform.GetPositionSnap();
	glm::vec3 boardScale = transform.GetLocalScale();

	float left = (boardScale.x / 2.f) * -1.f;
	float right = boardScale.x / 2.f;
	float top = boardScale.y / 2.f;
	float bottom = (boardScale.y / 2.f) * -1.f;

	float boardLeft = boardPos.x + left;
	float boardRight = boardPos.x + right;
	float boardTop = boardPos.y + top;
	float boardBottom = boardPos.y + bottom;

	if (Pos.y >= boardBottom - 1 && Pos.y < boardTop &&
		Pos.x >= boardLeft && Pos.x < boardRight)
	{
		Pos = ConvertBoardCoord(Pos);
		int bordCol = (int)Pos.x;
		int bordRow = (int)Pos.y;

		if (!Containers[bordRow][bordCol]->GetEmpty())
		{
			res = true;
		}
	}

	return res;
}

bool TetrisBoard::collisionVertical(TetrisBlock &block)
{
	glm::vec3 boardPos = transform.GetPositionSnap();
	glm::vec3 boardScale = transform.GetLocalScale();

	float bottom = (boardScale.y / 2.f) * -1.f;
	float boardBottom = boardPos.y + bottom;
	
	for (int i = 0; i < 4; i++)
	{
		glm::vec3 Pos = block.blocks[i].transform.GetPositionSnap();
		
		Pos.x = Pos.x + 0;
		Pos.y = Pos.y - 1;

		if (Pos.y < boardBottom || collisionSettleBlcoks(Pos))
		{
			return true;
		}
	}

	return false;
}



bool TetrisBoard::collisionLeftWall(TetrisBlock& block)
{
	glm::vec3 boardPos = transform.GetPositionSnap();
	glm::vec3 boardScale = transform.GetLocalScale();

	float left = (boardScale.x / 2.f) * -1.f;
	float boardLeft = boardPos.x + left;
	
	for (int i = 0; i < 4; i++)
	{
		glm::vec3 Pos = block.blocks[i].transform.GetPositionSnap();

		int x = (int)Pos.x - 1;

		glm::vec3 Pos2(x, Pos.y, Pos.z);
		if (x < boardLeft || collisionSettleBlcoks(Pos2))
		{
			return true;
		}
	}
	return false;
}

bool TetrisBoard::collisionRightWall(TetrisBlock& block)
{
	glm::vec3 boardPos = transform.GetPositionSnap();
	glm::vec3 boardScale = transform.GetLocalScale();

	float right = boardScale.x / 2.f;
	float boardRight = boardPos.x + right;

	for (int i = 0; i < 4; i++)
	{
		glm::vec3 Pos = block.blocks[i].transform.GetPositionSnap();

		Pos.x = Pos.x + 1;

		if (Pos.x >= boardRight || collisionSettleBlcoks(Pos))
		{
			return true;
		}
	}
	return false;
}

bool TetrisBoard::collisionRotate(TetrisBlock& block)
{
	glm::vec3 boardPos = transform.GetPositionSnap();
	glm::vec3 boardScale = transform.GetLocalScale();

	float left = (boardScale.x / 2.f) * -1.f;
	float right = boardScale.x / 2.f;
	float bottom = (boardScale.y / 2.f) * -1.f;

	float boardLeft = boardPos.x + left;
	float boardRight = boardPos.x + right;
	float boardBottom = boardPos.y + bottom;

	for (int i = 0; i < 4; i++)
	{
		glm::vec3 Pos = block.blocks[i].transform.GetPositionSnap();

		if (Pos.x < boardLeft || 
			Pos.x >= boardRight ||
			Pos.y <= boardBottom ||
			collisionSettleBlcoks(Pos))
		{
			return true;
		}
	}
	return false;
}

glm::vec3 TetrisBoard::GetHitBottomPos(TetrisBlock& Block)
{
	glm::vec3 pos(0.0f);
	while (!collisionVertical(Block))
	{
		pos = Block.transform.GetPositionSnap();
		pos.y = pos.y + -1.0f;
		Block.transform.SetWorldPosition(pos.x, pos.y, pos.z);
		Block.initTetrisBlockShape();
	}
	return pos;
}


glRect2D& TetrisBoard::GetBlock(glRect2D& block)
{
	glRect2D* resblock = nullptr;

	if (BlocksPool.size() > 0)
	{
		resblock = BlocksPool.back();
		BlocksPool.pop_back();
	}
	else
	{
		resblock = (glRect2D*)&SceneManager::CreateObject(OBJECTTYPE::PLANE);
		SceneManager::GetRootNode().AddChild(*resblock);
	}

	resblock->Name = "Settled Block Element";
	glm::vec3 pos = block.transform.GetPositionSnap();
	resblock->transform.SetWorldPosition(pos.x, pos.y, pos.z);
	resblock->color = block.color;
	resblock->ReCompileShader(block.GetVertexShader(), block.GetFragmentShader());

	return *resblock;
}

void TetrisBoard::AddBlock(glRect2D& block)
{
	BlocksPool.push_back(&block);
}