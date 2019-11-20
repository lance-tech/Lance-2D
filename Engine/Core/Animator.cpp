#include "Animator.h"

Animator::Animator() :
	Time(0),
	EndTime(0),
	isFinish(false),
	object(nullptr)   
{
}

Animator::~Animator()
{
}

void Animator::SetTargetObject(Object& object)
{
	this->object = &object;
	Time = 0;
	EndTime = animation.GetEndTime() + 1;
	isFinish = false;
}

void Animator::UpdateAnimation()
{
	if (Time < EndTime)
	{
		if (animation.TranslateKeys.size() > 0)
		{
			glm::vec4 outValue = Animation::GetValueOnTime(Time, animation.TranslateKeys);
			object->transform.SetWorldPosition(outValue.x, outValue.y, outValue.z);
		}

		if (animation.RotateKeys.size() > 0)
		{
			glm::vec4 outValue = Animation::GetValueOnTime(Time, animation.RotateKeys);
			object->transform.SetWorldRotate(outValue.x, outValue.y, outValue.z);
		}
		if (animation.ScaleKeys.size() > 0)
		{
			glm::vec4 outValue = Animation::GetValueOnTime(Time, animation.ScaleKeys);
			object->transform.SetWorldScale(outValue.x, outValue.y, outValue.z);
		}
		if (animation.ColorKeys.size() > 0)
		{
			glm::vec4 outValue = Animation::GetValueOnTime(Time, animation.ColorKeys);
			object->color.SetColor(outValue.x, outValue.y, outValue.z, outValue.w);
		}

		Time++;
	}
	else
	{
		isFinish = true;
		glm::vec3 lastPos = animation.TranslateKeys[animation.TranslateKeys.size() - 1].Value;
		animation.TranslateKeys.clear();
		animation.RotateKeys.clear();
		animation.ScaleKeys.clear();
		animation.ColorKeys.clear();
		object->transform.SetWorldPosition(lastPos.x, lastPos.y, lastPos.z);
	}
}
