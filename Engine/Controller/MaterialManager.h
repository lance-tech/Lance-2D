#pragma once
#include <vector>
#include <string>
#include <unordered_map>
#include <GL/glew.h>
#include "../Core/Shader.h"
#include "../GLObject/Object.h"

struct MatchID
{
	MatchID(const unsigned int id) : id_(id) {}
	bool operator()(const Shader* obj) const
	{
		return obj->GetID() == id_;
	}
private:
	const unsigned int id_;
};


class MaterialManager
{
public:
	MaterialManager();
	~MaterialManager();

	static MaterialManager* GetInstance();

	static Shader& CreateShader(const char* v_path, const char* f_path, bool refShader = true);
	Shader& GetShader(const unsigned int ID);
	static bool ReCompileShader(const unsigned int id, const char* v_path, const char* f_path);

private:
	std::vector<Shader*> SceneShaders;
	static bool Instantiated;
	static MaterialManager* Instance;
};

