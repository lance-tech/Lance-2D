#pragma once

#include <utility>
#include <vector>
#include <algorithm>
#include <string>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Keyframe.h"
#include "../GLObject/Object.h"

class Animator
{
public:
	Animator();
	~Animator();

	void SetTargetObject(Object& object);
	void UpdateAnimation();

	Animator& operator=(const Animator &other)
	{
		this->object = other.object;
		this->Time = other.Time;
		this->EndTime = other.EndTime;
		this->isFinish = other.isFinish;

		return *this;
	}

	float Time;
	float EndTime;
	bool isFinish;
	Object* object;
	Animation animation;
};
