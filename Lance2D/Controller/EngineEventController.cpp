#include "EngineEventController.h"
#include <qdebug.h>

EngineEventController::EngineEventController()
	: engineEventModel(nullptr)
{
}

EngineEventController::EngineEventController(EngineEventModel &model)
	: engineEventModel(&model)
{
}

EngineEventController::~EngineEventController()
{
}

void EngineEventController::DeleteObjectEvent(std::string name)
{
	Node& delNode = SceneManager::GetSceneRoot().FindNode(name);
	delNode.parent().RemoveSubNodes(delNode);
	engineEventModel->SetObject(nullptr);
}

Object& EngineEventController::NewObjectEvent(OBJECTTYPE obejctType)
{
	return SceneManager::CreateObject(obejctType, nullptr, nullptr, false);
}

Object& EngineEventController::CopyObjectEvent(const Object& sourceObject)
{
	return SceneManager::CopyObject(sourceObject);
}

void EngineEventController::OpenScene(std::string filename)
{
	GameScene* currentScene = SceneManager::GetCurrentScene();
	FileManager::OpenScene(filename, currentScene);
	engineEventModel->SetTimeModel(currentScene->timeline);
}

void EngineEventController::SaveScene(std::string filename, Node& sceneRoot)
{
	EngineData data;
	data.TotalFrame = engineEventModel->Timeline->GetFrameCount();
	data.StartFrame = engineEventModel->Timeline->StartFrame;
	data.EndFrame = engineEventModel->Timeline->EndFrame;

	FileManager::SetData(data);

	bool res = FileManager::SaveScene(filename, sceneRoot);

	if (!res)
	{
#ifdef ENGINE_MODE
		qDebug() << "Failed to save.";
#endif
	}
}