#include "GameMainScene.h"
#include <cmath>

GameMainScene::GameMainScene() : 
	boardBehaviour(nullptr), blockBehaviour(nullptr),
	ActiveBlock(nullptr), guideTBlock(nullptr), ghostTBlock(nullptr),
	GameBoard(nullptr), l_level(nullptr), l_score(nullptr),
	level(0), score(0),
	lastTime(0.0), timeAccum(0.0), interval(0.3)
{
	queueNode[0] = nullptr;
	queueNode[1] = nullptr;
	queueNode[2] = nullptr;

	FileManager::OpenScene("Assets/Level/Main Scene.txt", this);
	
	Node& GameBoardNode = SceneRoot->FindNode("GameBoard");
	Object& gameBoardObject = GameBoardNode.GetNodeObject();
	
	GameBoard = new TetrisBoard(gameBoardObject);
	Shader& shader = MaterialManager::CreateShader("Assets/shaders/TetrisBoard.vert", "Assets/shaders/TetrisBoard.frag");
	GameBoard->SetShader(shader);
	GameBoard->Name = "Lance Game Board";
	GameBoardNode.DestroyNode(GameBoardNode);


	boardBehaviour = new BoardBehaviour();
	boardBehaviour->Name = "BoardBehaviour";
	boardBehaviour->GameBoard = GameBoard;
	GameBoard->AddComponent(boardBehaviour);
	SceneRoot->RootNode->AddChild(*GameBoard);

	l_score = (glLabel*)&SceneRoot->FindNode("Score").GetNodeObject();
	l_level = (glLabel*)&SceneRoot->FindNode("Level").GetNodeObject();

	queueNode[0] = &SceneRoot->FindNode("BlockQueue1");
	queueNode[1] = &SceneRoot->FindNode("BlockQueue2");
	queueNode[2] = &SceneRoot->FindNode("BlockQueue3");
}

void GameMainScene::InitializeGame()
{
	blockBehaviour = new BlockBehaviour();
	blockBehaviour->Name = "BlockBehaviour";

	level = 1;
	score = 0;

	l_level->SetText(std::to_string(level));
	l_score->SetText(std::to_string(score));

	timeAccum = 0.0;
	lastTime = SceneManager::GetInstance()->GetTickTime();

	for (int i = 0; i < 3; i++)
	{
		TetrisBlock &tBlock = (TetrisBlock&)SceneManager::CreateObject((OBJECTTYPE)(rand() % 7));
		SceneRoot->RootNode->AddChild(tBlock);
		BlockQueue.push_back(&tBlock);
	}

	glm::vec3 boardPos = GameBoard->transform.GetPositionSnap();
	glm::vec3 boardScale = GameBoard->transform.GetLocalScale();
	float top = boardScale.y / 2.f;

	guideTBlock = (TetrisBlock*)&SceneManager::CreateObject(OBJECTTYPE::block0);
	SceneRoot->RootNode->AddChild(*guideTBlock);
	guideTBlock->Name = "Guide Block";
	guideTBlock->color.SetColor(0.5f, 0.5f, 0.5f, 0.75f);
	guideTBlock->transform.SetWorldPosition(boardPos.x, boardPos.y + top + 1.f);

	ghostTBlock = (TetrisBlock*)&SceneManager::CreateObject(OBJECTTYPE::block0);
	SceneRoot->RootNode->AddChild(*ghostTBlock);
	ghostTBlock->SetVisible(false);
	ghostTBlock->Name = "Ghost Block";
	ghostTBlock->color.SetColor(0.5f, 0.5f, 0.5f, 0.75f);
	ghostTBlock->transform.SetWorldPosition(boardPos.x, boardPos.y + top + 1.f);
	GameBoard->GetHitBottomPos(*ghostTBlock);

	ActiveBlock = CreateTetrisBlock();
}

void GameMainScene::DestroyGame()
{
	boardBehaviour->ClearBoard();

	for (const auto& block : BlockQueue)
	{
		Node& node = SceneRoot->FindNode(*block);
		node.DestroyNode(node);
	}
	BlockQueue.clear();

	if (ActiveBlock)
	{
		Node& node1 = SceneRoot->FindNode(*ActiveBlock);
		node1.DestroyNode(node1);
		ActiveBlock = nullptr;

		Node& node2 = SceneRoot->FindNode(*guideTBlock);
		node2.DestroyNode(node2);
		guideTBlock = nullptr;

		Node& node3 = SceneRoot->FindNode(*ghostTBlock);
		node3.DestroyNode(node3);
		ghostTBlock = nullptr;
	}
}

GameMainScene::~GameMainScene()
{
	DestroyGame();
}

void GameMainScene::GameOver()
{
	GameDataManager::SaveGameData();

	DestroyGame();

	SceneManager::SetNextScene();
}

TetrisBlock* GameMainScene::CreateTetrisBlock()
{
	TetrisBlock* newBlock = BlockQueue.front();
	newBlock->Name = "Active Block";

	glm::vec3 boardPos = GameBoard->transform.GetPositionSnap();
	glm::vec3 boardScale = GameBoard->transform.GetLocalScale();
	float top = boardScale.y / 2.f;

	newBlock->transform.SetWorldPosition(boardPos.x, boardPos.y + top + 1.f);


	blockBehaviour->Block = newBlock;
	newBlock->AddComponent(blockBehaviour);

	BlockQueue.pop_front();
	
	TetrisBlock &nextBlock = (TetrisBlock&)SceneManager::CreateObject((OBJECTTYPE)(rand() % 7));
	SceneRoot->RootNode->AddChild(nextBlock);
	BlockQueue.push_back(&nextBlock);

	for (int i = 0; i < 3; i++)
	{
		Object &object = queueNode[i]->GetNodeObject();
		glm::vec3 pos = object.transform.GetPositionSnap();
		BlockQueue[i]->transform.SetWorldPosition(pos.x, pos.y, pos.z + 0.5f);
	}

	ghostTBlock->Type = newBlock->Type;
	ghostTBlock->transform.SetWorldPosition(boardPos.x, boardPos.y + top + 1.f);

	guideTBlock->Type = newBlock->Type;
	guideTBlock->transform.SetWorldPosition(boardPos.x, boardPos.y + top + 1.f);

	return newBlock;
}

void GameMainScene::MoveHorizontal(int direction)
{
	if (direction < 0 && GameBoard->collisionLeftWall(*ActiveBlock))
		return;
	if (direction > 0 && GameBoard->collisionRightWall(*ActiveBlock))
		return;

	blockBehaviour->MoveHorizontal(direction);
}

void GameMainScene::Rotate()
{
	if (ActiveBlock->Type == OBJECTTYPE::block6)
		return;

	blockBehaviour->Rotate(90.f);
	if (GameBoard->collisionRotate(*ActiveBlock))
	{
		blockBehaviour->Rotate(-90.f);
	}
}

void GameMainScene::DownStraight()
{
	while (!GameBoard->collisionVertical(*ActiveBlock)) 
	{
		blockBehaviour->MoveVertical(-1.0f);
	}

	if (GameBoard->CheckRowFull(*ActiveBlock))
	{
		GameOver();
		return;
	}

	GameBoard->SettleLogic(*ActiveBlock);

	// Create New Active Block
	ActiveBlock = CreateTetrisBlock();

	// Update Score
	IncreaseScore();
}

void GameMainScene::MoveVertical(GLfloat y)
{
	bool res = GameBoard->collisionVertical(*ActiveBlock);
	if (res)
	{
		if (GameBoard->CheckRowFull(*ActiveBlock))
		{
			GameOver();
			return;
		}

		GameBoard->SettleLogic(*ActiveBlock);

		// Create New Active Block
		ActiveBlock = CreateTetrisBlock();

		// Update Score
		IncreaseScore();
	}
	else
	{
		blockBehaviour->MoveVertical(y);
	}
}

void GameMainScene::IncreaseScore()
{
	score++;
	l_score->SetText(std::to_string(score));
	GameDataManager::SetUserScore(score);
}

bool GameMainScene::CheckLevel(double time)
{
	unsigned int levelLimit = level * 20;
	if (score > levelLimit) {
		level++;
		l_level->SetText(std::to_string(level));
		GameDataManager::SetUserLevel(level);
	}

	interval = 1.0 / (double)level;
	double deltaTime = time - lastTime;
	timeAccum += deltaTime;

	if (timeAccum >= interval)
	{
		timeAccum = 0.0;
		return true;
	}
	lastTime = time;
	return false;
}

void GameMainScene::Render(glm::mat4 &VP, double &time)
{
	GameScene::Render(VP, time);
}

void GameMainScene::GameLogic(double &time)
{
	if (ActiveBlock)
	{
		ghostTBlock->transform = ActiveBlock->transform;
		ghostTBlock->initTetrisBlockShape();
		GameBoard->GetHitBottomPos(*ghostTBlock);
		guideTBlock->transform.WorldMatrix = ghostTBlock->transform.WorldMatrix;
	}
	if (CheckLevel(time))
	{
		MoveVertical(-1);
	}
}

void GameMainScene::OnKeyPressed(int Event)
{
	if (ActiveBlock)
	{
		switch (Event)
		{
		case KEY_UP:			
			Rotate();
			break;
		case KEY_DOWN:
			MoveVertical(-1);
			break;
		case KEY_LEFT:
			MoveHorizontal(-1);
			break;
		case KEY_RIGHT:
			MoveHorizontal(1);
			break;
		case KEY_SPACE:
			DownStraight();
			break;
		default:
			break;
		}
	}
}
