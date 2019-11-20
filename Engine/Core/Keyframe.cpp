#include "Keyframe.h"



Keyframe::Keyframe() : Time(0.f), Value(0.0f), Index(-1)
{
}


Keyframe::~Keyframe()
{
}


Keyframe::Keyframe(int index, float time, glm::vec4 value)
{
	Index = index;
	Time = time;
	Value = value;
}
