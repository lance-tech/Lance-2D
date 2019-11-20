#pragma once

#include "../Engine.h"
#include <string>
#include <algorithm>

// GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/euler_angles.hpp>

// Other
#include "../Utils/Delegate.h"
#include "../Core/Transform.h"
#include "../Core/Color.h"
#include "../Core/Animation.h"
#include "../GameComponent/GameComponent.h"
class Shader;

class Object
{
public:
	Object();
	Object& operator=(const Object &other);
	virtual ~Object();

	virtual void pressed();
	virtual void Draw(glm::mat4 &VP, double &time) = 0;
	void UpdateComponents(double Time);

	template <typename T, typename Method>
	void Connect(T* object, Method method)
	{
		this->_callBack.Connect(object, method);
	}

	virtual void SetShader(Shader& shader) = 0;
	virtual void DestroyShader() {};
	virtual void ReCompileShader(const char *Vfilename = nullptr, const char *Ffilename = nullptr) = 0;
	virtual const char* GetVertexShader() = 0;
	virtual const char* GetFragmentShader() = 0;
	virtual Shader* GetShader() = 0;
	virtual void SetTexture(char *path) = 0;

protected:
	Delegate _callBack;
	std::vector<GameComponent*> Components;

public:
	void AddComponent(GameComponent* component);
	void RemoveComponent(GameComponent* component);
	GameComponent* GetComponent(std::string name);
	std::vector<GameComponent*>& GetComponents();

	void SetSelected(bool isSelected);
	bool IsSelected;
	OBJECTTYPE Type;
	std::string Name;

	Core::Transform transform;
	Color color;
	Color selectColor;
	Animation animation;

	bool GetVisible();
	void SetVisible(bool state);
private:
	bool visible;
};