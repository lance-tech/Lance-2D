#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Keyframe
{
public:
	Keyframe();
	~Keyframe();

	Keyframe(int index, float time, glm::vec4 value);

	float Time;
	glm::vec4 Value;
	int Index;

	Keyframe& operator=(const Keyframe &other)
	{
		this->Time = other.Time;
		this->Value = other.Value;
		this->Index = other.Index;
		return *this;
	}
};

