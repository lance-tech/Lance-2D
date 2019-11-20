#pragma once
#include <vector>
#include <string>
#include "../Tools/EngineEventModel.h"
#include <Controller/SceneManager.h>
#include <Core/FileManager.h>
#include <GLObject/Object.h>

class EngineEventController
{
public:
	EngineEventController();
	explicit EngineEventController(EngineEventModel &model);
	~EngineEventController();

	EngineEventModel *engineEventModel;

	void DeleteObjectEvent(std::string name);
	Object& NewObjectEvent(OBJECTTYPE obejctType);
	Object& CopyObjectEvent(const Object& sourceObject);

	void SaveScene(std::string filename, Node& sceneRoot);
	void OpenScene(std::string filename);
};

