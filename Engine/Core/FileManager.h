#pragma once

#include <list>
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>

#include "../Engine.h"

#include "../Controller/SceneManager.h"
#include "../Utils/StringUtils.h"
#include "Transform.h"
#include "../Scene/GameScene.h"
#include "../GameObject/TetrisBlock.h"
#include "../GLObject/Object.h"
#include "../GLObject/glMesh.h"
#include "../GLObject/Grid.h"
#include "../GLObject/glButton.h"
#include "../GLObject/glLabel.h"

#include "Node.h"
#include "Color.h"
#include "Keyframe.h"


typedef struct _engineData
{
	int StartFrame;
	int EndFrame;
	int TotalFrame;
} EngineData;


class FileManager
{
public:
	FileManager();
	~FileManager();
	
	static bool SaveScene(const std::string filename, Node& node);
	static bool OpenScene(const std::string filename, GameScene* scene);

	static void SetData(EngineData& data);
	static EngineData& GetData();

private:
	static EngineData Data;
};

