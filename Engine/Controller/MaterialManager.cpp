#include "MaterialManager.h"
#include <cstdlib>
#include <ctime>


MaterialManager* MaterialManager::Instance = nullptr;
bool MaterialManager::Instantiated = false;

MaterialManager::MaterialManager()
{
	assert(!Instantiated);
	Instantiated = true;
	Instance = this;
	std::srand(static_cast<unsigned int>(std::time(0)));
}

MaterialManager::~MaterialManager()
{
	while (GetInstance()->SceneShaders.size() > 0)
	{
		Shader *shader = GetInstance()->SceneShaders.back();
		delete shader;
		shader = nullptr;

		GetInstance()->SceneShaders.pop_back();
	}
}

MaterialManager* MaterialManager::GetInstance()
{
	if (!Instantiated) {
		Instance = new MaterialManager();
		Instantiated = true;
	}
	return Instance;
}

bool MaterialManager::ReCompileShader(const unsigned int id, const char* v_path, const char* f_path)
{
	bool result = false;

	Shader &shader = GetInstance()->GetShader(id);
	if (&shader)
	{
		std::string v = v_path;
		std::string f = f_path;
		std::hash<std::string> hasher;
		unsigned int ID = (unsigned int)hasher(v + f);

		shader.SetID(ID);
		shader.ReCompileShader(v_path, f_path);

		result = true;
	}

	return result;
}

Shader& MaterialManager::CreateShader(const char* v_path, const char* f_path, bool refShader)
{
	MaterialManager* instance = GetInstance();
	std::string v = v_path;
	std::string f = f_path;
	std::hash<std::string> hasher;
	unsigned int ID = (unsigned int)hasher(v + f);

	if (!refShader)
	{
		ID -= std::rand();
	}

	Shader *shader = &instance->GetShader(ID);
	if (!shader)
	{
		shader = new Shader(ID, v_path, f_path);
		instance->SceneShaders.push_back(shader);
	}

	return *shader;
}

Shader& MaterialManager::GetShader(const unsigned int ID)
{
	Shader* shader = nullptr;

	std::vector<Shader*>::iterator it = std::find_if(
		SceneShaders.begin(),
		SceneShaders.end(), MatchID(ID));

	if (it != SceneShaders.end())
	{
		shader = *it;
	}

	return *shader;
}

