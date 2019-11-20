#include "Object.h"


Object::Object() : 
	IsSelected(false),
	Type(), 
	Name("Object"), 
	visible(true)
{	
	selectColor.SetColor(0.0f, 0.0f, 0.0f);
}

Object & Object::operator=(const Object & other)
{
	this->_callBack = other._callBack;
	this->Components = other.Components;
	this->IsSelected = other.IsSelected;
	this->Type = other.Type;
	this->transform = other.transform;
	this->color = other.color;
	this->selectColor = other.selectColor;
	this->animation = other.animation;
	this->Name = other.Name;
	return *this;
}

void Object::UpdateComponents(double Time)
{
	for (auto const &component : Components)
	{
		component->Update(Time);
	}
}

void Object::AddComponent(GameComponent* component)
{
	Components.push_back(component);
}

void Object::RemoveComponent(GameComponent* component)
{
	std::vector<GameComponent*>::iterator it = std::find(Components.begin(), Components.end(), component);
	if (it != Components.end())
	{
		Components.erase(it);
	}
}

GameComponent* Object::GetComponent(std::string name)
{
	GameComponent* component = nullptr;
	std::vector<GameComponent*>::iterator it = std::find_if(Components.begin(), Components.end(), CompareName(name));
	if (it != Components.end())
	{
		component = *it;
	}

	return component;
}

std::vector<GameComponent*>& Object::GetComponents()
{
	return Components;
}

Object::~Object()
{
	while (!Components.empty())
	{
		GameComponent* component = Components.back();
		Components.pop_back();

		delete component;
	}

	DestroyShader();
}

void Object::pressed()
{
	if (_callBack.IsNull())
		return;

	this->_callBack();
}


void Object::SetSelected(bool isSelected)
{
	IsSelected = isSelected;

	if (isSelected)
	{
		selectColor.SetColor(1.0f, -1.0f, -1.0f);
	}
	else
	{
		selectColor.SetColor(0.0f, 0.0f, 0.0f);
	}
}

bool Object::GetVisible()
{
	return visible;
}

void Object::SetVisible(bool state)
{
	visible = state;
}