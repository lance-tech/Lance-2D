#include "BoardBehaviour.h"



BoardBehaviour::BoardBehaviour() : GameBoard(nullptr)
{
}


BoardBehaviour::~BoardBehaviour()
{
}

void BoardBehaviour::SetSettleBlock(TetrisBlock& block)
{
	// Set Arrive
	for (int i = 0; i < 4; i++)
	{
		glRect2D& settleBlock = GameBoard->GetBlock(block.blocks[i]);
		glm::vec3 pos = settleBlock.transform.GetPositionSnap();

		pos = GameBoard->ConvertBoardCoord(pos);
		int row = (int)pos.y;
		int column = (int)pos.x;


		GameBoard->SetBlock(row, column, settleBlock);
	}
}

bool BoardBehaviour::CheckBoardFull()
{
	bool isFull = false;

	//BlockContainer** containers = GameBoard->GetContainers();

	for (unsigned int row = 0; row < GameBoard->BoardRow; row++)
	{
		if (GameBoard->IsRowFull(row))
		{
			isFull = true;

			for (unsigned int col = 0; col < GameBoard->BoardColumn; col++)
			{
				GameBoard->Containers[row][col]->SetDelete(true);
				
				glRect2D& block = GameBoard->Containers[row][col]->GetBlock();
				GameBoard->AddBlock(block);
			}
		}
	}
	return isFull;
}


void BoardBehaviour::SetBlocksAnimation()
{
	glm::vec3 boardPos = GameBoard->transform.GetPositionSnap();
	glm::vec3 scale = GameBoard->transform.GetLocalScale();

	float bottom = (scale.y / 2.f) * -1.f;
	float left = (scale.x / 2.f) * -1.f;
	float right = scale.x / 2.f;

	float boardBottom = boardPos.y + bottom;
	float boardLeft = boardPos.x + left;
	float boardRight = boardPos.x + right;

	int delCount = 0;
	for (unsigned int row = 0; row < GameBoard->BoardRow; row++)
	{
		if (GameBoard->Containers[row][0]->GetDelete())
		{
			delCount++;
		}

		for (unsigned int col = 0; col < GameBoard->BoardColumn; col++)
		{
			glRect2D& block = GameBoard->Containers[row][col]->GetBlock();

			float x = boardLeft + col;
			float y = boardBottom + row;
			float z = 0.0f;

			if (GameBoard->Containers[row][col]->GetDelete())
			{
				Animator* animator = new Animator();

				// Set Delete Animation
				animator->animation.SetTranslateKey(0.0f + col + row, glm::vec4(x, y, z, 1.0f));
				Color& color = block.color.GetColor();
				animator->animation.SetColorKey(5.0f + col + row, glm::vec4(color.R, color.G, color.B, 1.0f));

				if (row % 2 == 0)
				{
					animator->animation.SetTranslateKey(10.0f + col + row, glm::vec4(boardLeft - 5, y, z, 1.0f));
				}
				else
				{
					animator->animation.SetTranslateKey(10.0f + col + row, glm::vec4(boardRight + 5, y, z, 1.0f));
				}
				animator->animation.SetColorKey(11.0f + col + row, glm::vec4(color.R, color.G, color.B, 0.0f));

				animator->SetTargetObject(block);
				SceneManager::animationManager.AddAnimator(*animator);
			}

			else if (!GameBoard->Containers[row][col]->GetEmpty() && delCount > 0)
			{
				Animator* animator = new Animator();

				// Set Shift Down Animation
				animator->animation.SetTranslateKey(0.0f + col + row, glm::vec4(x, y, z, 1.0f));
				animator->animation.SetTranslateKey(3.0f + col + row, glm::vec4(x, y + 0.4f, z, 1.0f));
				animator->animation.SetTranslateKey(7.0f + col + row, glm::vec4(x, y - delCount - 0.6f, z, 1.0f));
				animator->animation.SetTranslateKey(9.0f + col + row, glm::vec4(x, y - delCount + 0.3f, z, 1.0f));
				animator->animation.SetTranslateKey(10.0f + col + row, glm::vec4(x, y - delCount, z, 1.0f));

				animator->animation.SetRotateKey(6.0f + col + row, glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
				animator->animation.SetRotateKey(9.0f + col + row, glm::vec4(0.0f, 0.0f, 10.0f, 1.0f));
				animator->animation.SetRotateKey(11.0f + col + row, glm::vec4(0.0f, 0.0f, -10.0f, 1.0f));
				animator->animation.SetRotateKey(13.0f + col + row, glm::vec4(0.0f, 0.0f, 5.0f, 1.0f));
				animator->animation.SetRotateKey(14.0f + col + row, glm::vec4(0.0f, 0.0f, -2.0f, 1.0f));
				animator->animation.SetRotateKey(15.0f + col + row, glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
				
				animator->SetTargetObject(block);
				SceneManager::animationManager.AddAnimator(*animator);
			}
		}
	}
}

void BoardBehaviour::UpdateBoardState()
{
	int delCount = 0;

	for (unsigned int row = 0; row < GameBoard->BoardRow; row++)
	{
		if (GameBoard->Containers[row][0]->GetDelete())
		{
			delCount++;
		}

		for (unsigned int column = 0; column < GameBoard->BoardColumn; column++)
		{
			glRect2D& block = GameBoard->Containers[row][column]->ExtractBlock();

			if (GameBoard->Containers[row][column]->GetDelete())
			{
				GameBoard->Containers[row][column]->SetEmpty(true);
				GameBoard->Containers[row][column]->SetDelete(false);
			}

			else if (!GameBoard->Containers[row][column]->GetEmpty())
			{
				GameBoard->Containers[row][column]->SetEmpty(true);
				GameBoard->Containers[row - delCount][column]->SetSettlBlock(block);
			}
		}
	}
}

void BoardBehaviour::ClearBoard()
{
	for (unsigned int row = 0; row < GameBoard->BoardRow; row++)
	{
		for (unsigned int col = 0; col < GameBoard->BoardColumn; col++)
		{
			GameBoard->Containers[row][col]->SetEmpty(true);
			GameBoard->Containers[row][col]->SetDelete(false);

			if (GameBoard->Containers[row][col]->HasBlock(row, col))
			{
				glRect2D& block = GameBoard->Containers[row][col]->GetBlock();
				Node& node = SceneManager::GetSceneRoot().FindNode(block);
				node.DestroyNode(node);
				GameBoard->Containers[row][col]->ExtractBlock();
			}
		}
	}

	for (const auto& block : GameBoard->BlocksPool)
	{
		Node& node = SceneManager::GetSceneRoot().FindNode(*block);
		node.DestroyNode(node);
	}
	GameBoard->BlocksPool.clear();
}

void BoardBehaviour::Update(double &Time)
{
	//std::cout << "Pool : " << GameBoard->BlocksPool.size() << std::endl << std::endl;
}