#include "SceneManager.h"
#include "SceneGraph.h"

SceneManager* SceneManager::Instance = nullptr;
bool SceneManager::Instantiated = false;
AnimationManager SceneManager::animationManager;


void SceneManager::SetTickTime(double tickTime)
{
	Time = tickTime;
}

double SceneManager::GetTickTime()
{
	return GetInstance()->Time;
}

SceneManager* SceneManager::GetInstance()
{
	if (!Instantiated) {
		Instance = new SceneManager();
		Instantiated = true;
	}
	return Instance;
}

Object& SceneManager::CopyObject(const Object& sourceObject)
{
	Object& _sourceObject = const_cast<Object&>(sourceObject);

	Node& sourceNode = GetSceneRoot().FindNode(_sourceObject);
	sourceNode.ExecuteOneWay_B(&DuplicateNodes);

	Node& copyiedNode = GetSceneRoot().FindNode(sourceObject.Name + "_copyied");
	
	return copyiedNode.GetNodeObject();
}

Object& SceneManager::CreateObject(OBJECTTYPE objectType, const char* v_path, const char* f_path, bool refShader)
{
	Object* object = nullptr;
	switch (objectType)
	{
	case OBJECTTYPE::MESH:
	{
		glMesh *_mesh = new glMesh();
		const char *vp = "Assets/shaders/DirectionalLight.vert";
		const char *fp = "Assets/shaders/DirectionalLight.frag";
		if (v_path || f_path)
		{
			vp = v_path;
			fp = f_path;
		}
		Shader& shader = MaterialManager::CreateShader(vp, fp, refShader);
		_mesh->SetShader(shader);
		_mesh->IsSelected = false;
		_mesh->Type = OBJECTTYPE::MESH;
		_mesh->color.SetColor(0.75f, 0.75f, 0.75f, 1.0f);
		_mesh->transform.SetWorldPosition(0.0f, 0.0f);
		object = _mesh;
		break;
	}
	case OBJECTTYPE::BUTTON:
	{
		glButton *_button = new glButton("Text");
		const char *vp = "Assets/shaders/BevelBlock.vert";
		const char *fp = "Assets/shaders/BevelBlock.frag";
		if (v_path || f_path)
		{
			vp = v_path;
			fp = f_path;
		}
		Shader& shader = MaterialManager::CreateShader(vp, fp, refShader);
		_button->SetShader(shader);
		_button->IsSelected = false;
		_button->Type = OBJECTTYPE::BUTTON;
		_button->color.SetColor(0.75f, 0.75f, 0.75f, 1.0f);
		_button->SetTextColor(0.f, 0.f, 0.f);
		_button->SetFontSize(0.02f);
		_button->GetWidget()->transform.SetWorldPosition(0.0f, 0.0f);
		object = _button;
		break;
	}
	case OBJECTTYPE::LABEL:
	{
		glLabel *_label = new glLabel();
		const char *vp = "Assets/shaders/BevelBlock.vert";
		const char *fp = "Assets/shaders/BevelBlock.frag";
		if (v_path || f_path)
		{
			vp = v_path;
			fp = f_path;
		}
		Shader& shader = MaterialManager::CreateShader(vp, fp, refShader);
		_label->SetShader(shader);
		_label->IsSelected = false;
		_label->Type = OBJECTTYPE::LABEL;
		_label->color.SetColor(0.75f, 0.75f, 0.75f, 1.0f);
		_label->SetTextColor(0.f, 0.f, 0.f);
		_label->SetFontSize(0.02f);
		_label->GetWidget()->transform.SetWorldPosition(0.0f, 0.0f);
		_label->SetText("Text");
		object = _label;
		break;
	}
	case OBJECTTYPE::PLANE:
	{
		glRect2D *plane = new glRect2D();
		const char *vp = "Assets/shaders/rect2d.vert";
		const char *fp = "Assets/shaders/rect2d.frag";
		if (v_path || f_path)
		{
			vp = v_path;
			fp = f_path;
		}
		Shader& shader = MaterialManager::CreateShader(vp, fp, refShader);
		plane->SetShader(shader);
		plane->Type = OBJECTTYPE::PLANE;
		plane->IsSelected = false;
		plane->transform.SetTranslate(0, 0, 0);
		object = plane;
		break;
	}
	case OBJECTTYPE::GAMEBOARD:
	{
		Grid *_gameBoard = new Grid();
		const char *vp = "Assets/shaders/TetrisBoard.vert";
		const char *fp = "Assets/shaders/TetrisBoard.frag";
		if (v_path || f_path)
		{
			vp = v_path;
			fp = f_path;
		}
		Shader& shader = MaterialManager::CreateShader(vp, fp, refShader);
		_gameBoard->SetShader(shader);
		_gameBoard->Type = OBJECTTYPE::GAMEBOARD;
		_gameBoard->IsSelected = false;
		_gameBoard->transform.SetWorldPosition(-0.f, -0.0f, -1.9f);
		_gameBoard->color.SetColor(0.5f, 0.8f, 0.7f);
		object = _gameBoard;

		break;
	}
	default:
		TetrisBlock *tBlock = new TetrisBlock(objectType);
		const char *vp = "Assets/shaders/BlockElement.vert";
		const char *fp = "Assets/shaders/BlockElement.frag";
		if (v_path || f_path)
		{
			vp = v_path;
			fp = f_path;
		}
		Shader& shader = MaterialManager::CreateShader(vp, fp, refShader);
		tBlock->SetShader(shader);
		tBlock->IsSelected = false;
		tBlock->transform.SetWorldPosition(0, 0, 0);
		object = tBlock;
		break;
	}

	return *object;
}

SceneManager::SceneManager() : currentIndex(0), Time(0.0)
{
	assert(!Instantiated);
	Instantiated = true;
	Instance = this;
}


SceneManager::~SceneManager()
{
	for (std::vector<GameScene*>::iterator it = scenes.begin(); it != scenes.end(); ++it)
	{
		if (*it != NULL)
		{
			delete *it;
		}
	}
	Instantiated = false;
}

GameScene* SceneManager::GetCurrentScene()
{
	SceneManager* instance = GetInstance();
	return instance->scenes[instance->currentIndex];
}

void SceneManager::AddGameScene(GameScene &gameScene)
{
	GetInstance()->scenes.push_back(&gameScene);
}

void SceneManager::updateCurrentScene(glm::mat4 &VP, double& Time)
{
	SceneManager* instance = GetInstance();
	GameScene* currentScene = instance->scenes[instance->currentIndex];
	instance->SetTickTime(Time);

	if (currentScene->timeline.IsPlay)
	{
		currentScene->timeline.CurrentFrame++;
		if (currentScene->timeline.CurrentFrame > currentScene->timeline.GetFrameCount())
		{
			currentScene->timeline.CurrentFrame = 0;
		}
	}

	if (currentScene->timeline.IsPlay ||
		currentScene->timeline.IsTimelineMove)
	{
		double currentFrame = (double)currentScene->timeline.CurrentFrame;
		animationManager.Update(currentFrame, GetRootNode());
	}
	else
	{
		currentScene->UpdateTransform();
	}

	if (animationManager.IsAnimatorsPlay)
	{
		animationManager.UpdateAnimators();
	}

	currentScene->GameLogic(Time);
	currentScene->UpdateComponents(Time);

#ifdef GAME_MODE
	currentScene->Render(VP, Time);
#elif ENGINE_MODE
	double time = Time * 0.001f;
	currentScene->Render(VP, time);
#endif
	
}

void SceneManager::ResetScene()
{
	SceneManager* instance = GetInstance();
	instance->currentIndex = 0;
	instance->DoItFirst();
}

void SceneManager::SetNextScene()
{
	SceneManager* instance = GetInstance();
	instance->currentIndex++;
	instance->DoItFirst();
}

void SceneManager::DoItFirst()
{
	scenes[currentIndex]->InitializeGame();
	scenes[currentIndex]->timeline.IsPlay = true;
}

void SceneManager::SetRootNode(Node& rootNode)
{
	GetCurrentScene()->SceneRoot->RootNode = &rootNode;
}

Node& SceneManager::GetRootNode()
{
	return *GetCurrentScene()->SceneRoot->RootNode;
}

SceneGraph& SceneManager::GetSceneRoot()
{
	return *GetCurrentScene()->SceneRoot;
}

//
//void SceneManager::UpdateEvent(int event)
//{
//		if (currentNode->object)
//		{
//			for (auto const &GameComponent : Components)
//			{
//				(Event*)GameComponent->Update(EventType...);
//			}
//		}
//}