#include "FileManager.h"

EngineData FileManager::Data;

FileManager::FileManager()
{
}


FileManager::~FileManager()
{
}

void FileManager::SetData(EngineData& data)
{
	Data = data;
}

EngineData& FileManager::GetData()
{
	return Data;
}


bool FileManager::OpenScene(const std::string filename, GameScene* scene)
{
	Node* rootNode = scene->SceneRoot->RootNode;

	std::ifstream sceneFile(filename);
	std::string line;

	int totalFrame;
	std::getline(sceneFile, line);
	Utils::SubstrToInt(line, 7, &totalFrame);

	int startFrame;
	std::getline(sceneFile, line);
	Utils::SubstrToInt(line, 6, &startFrame);

	scene->timeline.StartFrame = startFrame;

	int endFrame;
	std::getline(sceneFile, line);
	Utils::SubstrToInt(line, 4, &endFrame);
	
	scene->timeline.EndFrame = endFrame;

	while (std::getline(sceneFile, line))
	{
		std::string::size_type n = line.find("type");

		if (n < line.length())
		{
			int typeRef;
			Utils::SubstrToInt(line, 5, &typeRef);
			OBJECTTYPE type = (OBJECTTYPE)typeRef;

			std::getline(sceneFile, line);
			std::string objectName = line.substr(5);

			std::getline(sceneFile, line);
			std::string parentName = line.substr(12);

			Node* parentNode = &scene->SceneRoot->FindNode(parentName);
			if (!parentNode)
			{
				parentNode = rootNode;
			}

			std::getline(sceneFile, line);
			std::vector<float> v_pos;
			Utils::SubstrToVector(line, 9, v_pos);
			glm::vec3 pos(v_pos[0], v_pos[1], v_pos[2]);


			std::getline(sceneFile, line);
			int keyCount;
			Utils::SubstrToInt(line, 10, &keyCount);

			std::vector<Keyframe> TranslateKeys;
			for (int i = 0; i < keyCount; i++)
			{
				std::getline(sceneFile, line);

				Keyframe key;
				key.Index = i;
				Utils::SubstrToKeyframe(line, &key);
				TranslateKeys.push_back(key);
			}


			std::getline(sceneFile, line);
			std::vector<float> v_rot;
			Utils::SubstrToVector(line, 7, v_rot);
			glm::vec3 rot(v_rot[0], v_rot[1], v_rot[2]);


			std::getline(sceneFile, line);
			keyCount;
			Utils::SubstrToInt(line, 10, &keyCount);

			std::vector<Keyframe> RotateKeys;
			for (int i = 0; i < keyCount; i++)
			{
				std::getline(sceneFile, line);

				Keyframe key;
				key.Index = i;
				Utils::SubstrToKeyframe(line, &key);
				RotateKeys.push_back(key);
			}



			std::getline(sceneFile, line);
			std::vector<float> v_scale;
			Utils::SubstrToVector(line, 6, v_scale);
			glm::vec3 scale(v_scale[0], v_scale[1], v_scale[2]);

			std::getline(sceneFile, line);
			keyCount;
			Utils::SubstrToInt(line, 10, &keyCount);

			std::vector<Keyframe> ScaleKeys;
			for (int i = 0; i < keyCount; i++)
			{
				std::getline(sceneFile, line);

				Keyframe key;
				key.Index = i;
				Utils::SubstrToKeyframe(line, &key);
				ScaleKeys.push_back(key);
			}


			std::getline(sceneFile, line);
			std::vector<float> v_color;
			Utils::SubstrToVector(line, 6, v_color);
			Color color(v_color[0], v_color[1], v_color[2], v_color[3]);

			std::getline(sceneFile, line);
			keyCount;
			Utils::SubstrToInt(line, 10, &keyCount);

			std::vector<Keyframe> ColorKeys;
			for (int i = 0; i < keyCount; i++)
			{
				std::getline(sceneFile, line);

				Keyframe key;
				key.Index = i;
				Utils::SubstrToKeyframe(line, &key);
				ColorKeys.push_back(key);
			}

			std::getline(sceneFile, line);
			float specular;
			Utils::SubstrToFloat(line, 9, &specular);

			std::getline(sceneFile, line);
			std::string vertexshader = line.substr(13);

			std::getline(sceneFile, line);
			std::string fragmentshader = line.substr(15);

			std::getline(sceneFile, line);
			std::string texture0 = line.substr(9);
			std::vector<char> convertConstStr(texture0.begin(), texture0.end());
			convertConstStr.push_back('\0');
			char* pTexture0 = &convertConstStr[0];
			
			Object& object = SceneManager::CreateObject(type, vertexshader.c_str(), fragmentshader.c_str(), false);
			
			parentNode->AddChild(object);
			
			Object &parentObject = parentNode->GetNodeObject();
			if (&parentObject)
			{
				object.transform.SetMatrix(parentObject.transform.WorldMatrix);
				object.transform.SetTranslate(pos.x, pos.y, pos.z);
				object.transform.SetScale(scale.x, scale.y, scale.z);
				object.transform.SetRotate(rot);
			}
			else 
			{
				object.transform.SetWorldPosition(pos.x, pos.y, pos.z);
				object.transform.SetWorldScale(scale.x, scale.y, scale.z);
				object.transform.SetWorldRotate(rot.x, rot.y, rot.z);
			}
			
			object.Name = objectName;
			object.GetShader()->SetTexture(pTexture0);
			object.IsSelected = false;
			object.Type = type;
			object.color = color;
			object.color.Specular = specular;
			object.animation.TranslateKeys = TranslateKeys;
			object.animation.RotateKeys = RotateKeys;
			object.animation.ScaleKeys = ScaleKeys;
			object.animation.ColorKeys = ColorKeys;

			if (type == OBJECTTYPE::MESH)
			{
				std::getline(sceneFile, line);
				std::string meshPath = line.substr(5);
				glMesh& mesh = dynamic_cast<glMesh&>(object);
				mesh.InitializeMesh(meshPath);
			}
			else if (type == OBJECTTYPE::BUTTON || type == OBJECTTYPE::LABEL)
			{
				glWidget& widget = dynamic_cast<glWidget&>(object);

				std::getline(sceneFile, line);
				std::string text = line.substr(5);

				std::getline(sceneFile, line);
				float fontSize;
				Utils::SubstrToFloat(line, 9, &fontSize);

				std::getline(sceneFile, line);
				std::vector<float> f_color;
				Utils::SubstrToVector(line, 10, f_color);
				Color fcolor(f_color[0], f_color[1], f_color[2], f_color[3]);

				widget.SetFontSize(fontSize);
				widget.SetText(text);
				widget.SetTextColor(fcolor.R, fcolor.G, fcolor.B, fcolor.A);
			}
			
		}
	}

	return true;
}

void ObjectDataWritter(Node& node, void* source1, void* source2)
{
	Object& object = node.GetNodeObject();
	std::ofstream* _file = static_cast<std::ofstream*>(source1);

	Node& parentNode = node.parent();
	Object& parentObject = parentNode.GetNodeObject();
	std::string parent = "";

	glm::vec3 localPos = object.transform.GetPosition();
	if (&parentNode && &parentObject)
	{
		parent = parentObject.Name;
		//localPos = object.transform.GetLocalPosition(parentObject.transform.WorldMatrix);
		localPos = object.transform.GetLocalPosition();
	}

	std::string name = object.Name;

	glm::vec3 scale;
	scale = object.transform.GetLocalScale();

	Color color = object.color.GetColor();
	glm::vec3 rot = object.transform.GetLocalRotate();
	OBJECTTYPE Type = object.Type;

	*_file << "type:" << (unsigned int)Type << "\n";
	*_file << "name:" << name << "\n";
	*_file << "parent name:" << parent << "\n";
	*_file << "position:" << localPos.x << "," << localPos.y << "," << localPos.z << "\n";
	*_file << "animation:" << object.animation.TranslateKeys.size() << "\n";
	for (int i = 0; i < object.animation.TranslateKeys.size(); i++)
	{
		Keyframe& key = object.animation.TranslateKeys[i];
		*_file << key.Time << ":" << key.Value.x << "," << key.Value.y << "," << key.Value.z << "\n";
	}
	*_file << "rotate:" << rot.x << "," << rot.y << "," << rot.z << "\n";
	*_file << "animation:" << object.animation.RotateKeys.size() << "\n";
	for (int i = 0; i < object.animation.RotateKeys.size(); i++)
	{
		Keyframe& key = object.animation.RotateKeys[i];
		*_file << key.Time << ":" << key.Value.x << "," << key.Value.y << "," << key.Value.z << "\n";
	}
	*_file << "scale:" << scale.x << "," << scale.y << "," << scale.z << "\n";
	*_file << "animation:" << object.animation.ScaleKeys.size() << "\n";
	for (int i = 0; i < object.animation.ScaleKeys.size(); i++)
	{
		Keyframe& key = object.animation.ScaleKeys[i];
		*_file << key.Time << ":" << key.Value.x << "," << key.Value.y << "," << key.Value.z << "\n";
	}
	*_file << "color:" << color.R << "," << color.G << "," << color.B << "," << color.A << "\n";
	*_file << "animation:" << object.animation.ColorKeys.size() << "\n";
	for (int i = 0; i < object.animation.ColorKeys.size(); i++)
	{
		Keyframe& key = object.animation.ColorKeys[i];
		*_file << key.Time << ":" << key.Value.r << "," << key.Value.g << "," << key.Value.b << "," << key.Value.a << "\n";
	}
	*_file << "specular:" << color.Specular << "\n";

	*_file << "vertexshader:" << object.GetVertexShader() << "\n";
	*_file << "fragmentshader:" << object.GetFragmentShader() << "\n";

	*_file << "texture0:" << object.GetShader()->GetTexturePath() << "\n";

	if (Type == OBJECTTYPE::BUTTON || Type == OBJECTTYPE::LABEL)
	{
		glWidget& widget = dynamic_cast<glWidget&>(object);
		float fontSize = widget.GetFontSize();
		std::string text = widget.GetText();
		Color textColor = widget.GetTextColor();

		*_file << "text:" << text << "\n";
		*_file << "fontsize:" << fontSize << "\n";
		*_file << "fontcolor:" << textColor.R << "," << textColor.G << "," << textColor.B << "," << textColor.A << "\n";
	}

	if (Type == OBJECTTYPE::MESH)
	{
		glMesh& mesh = dynamic_cast<glMesh&>(object);
		*_file << "mesh:" << mesh.GetMeshPath() << "\n";
	}

	*_file << "\n";
}

bool FileManager::SaveScene(const std::string filename, Node& node)
{
	bool res = true;
	std::ofstream sceneFile;
	sceneFile.exceptions(std::ofstream::failbit | std::ofstream::badbit);

	try
	{
		sceneFile.open(filename);

		sceneFile << "frames:" << Data.TotalFrame << "\n";
		sceneFile << "start:" << Data.StartFrame << "\n";
		sceneFile << "end:" << Data.EndFrame << "\n";

		node.ExecuteOneWay_D(&ObjectDataWritter, &sceneFile);
	
		sceneFile.close();
	}
	catch (std::ofstream::failure e)
	{
		//qDebug() << e.what();
		res = false;
	}

	return res;
}