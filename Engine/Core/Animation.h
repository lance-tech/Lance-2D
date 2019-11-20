#pragma once

#include <utility>
#include <vector>
#include <algorithm>
#include <string>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Keyframe.h"

struct MatchTime
{
	MatchTime(const float time) : time_(time) {}
	bool operator()(const Keyframe& obj) const
	{
		return obj.Time == time_;
	}
private:
	const float time_;
};

class Animation
{
public:
	Animation();
	~Animation();
	Animation& operator=(const Animation &other)
	{
		this->TranslateKeys = other.TranslateKeys;
		this->RotateKeys = other.RotateKeys;
		this->ScaleKeys = other.ScaleKeys;
		this->ColorKeys = other.ColorKeys;
		return *this;
	}

	bool HasKey();

	// TODO :
	// The key container need to change the base class and each property needs to subclassing.
	std::vector<Keyframe> TranslateKeys;
	std::vector<Keyframe> RotateKeys;
	std::vector<Keyframe> ScaleKeys;
	std::vector<Keyframe> ColorKeys;

	//void SetKey(std::string keyType, float time, float value);
	void SetTranslateKey(float time, glm::vec4 translate);
	void SetRotateKey(float time, glm::vec4 rotate);
	void SetScaleKey(float time, glm::vec4 scale);
	void SetColorKey(float time, glm::vec4 color);

	static void InsertKey(float time, glm::vec4 value, std::vector<Keyframe> &Keys);
	static Keyframe FindInKey(const float time, std::vector<Keyframe> &Keys);
	static glm::vec4 GetValueOnTime(const float time, std::vector<Keyframe> &Keys);

	float GetEndTime();
};