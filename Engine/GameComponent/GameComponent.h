#pragma once
#include <string>

struct CompareName;

class GameComponent
{
public:
	GameComponent();
	~GameComponent();

	std::string Name;
	virtual void Update(double &Time) = 0;
};

struct CompareName
{
	CompareName(const std::string name) : name_(name) {}
	bool operator()(const GameComponent* obj) const
	{
		return obj->Name == name_;
	}
private:
	const std::string name_;
};
