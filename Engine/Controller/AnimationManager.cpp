#include "AnimationManager.h"


AnimationManager::AnimationManager() : IsAnimatorsPlay(false)
{
}

AnimationManager::~AnimationManager()
{
}

void AnimationManager::UpdateAnimators()
{
	for (int i = 0; i < Animators.size(); i++)
	{
		Animator* animator = Animators[i];

		animator->UpdateAnimation();

		if (animator->isFinish)
		{
			animator->object = nullptr;
			delete animator;
			Animators.erase(Animators.begin() + i);
			--i;
		}
	}

	if (Animators.size() == 0)
	{
		IsAnimatorsPlay = false;
	}
}

void AnimationManager::PlayAnimators()
{
	IsAnimatorsPlay = true;
}

void AnimationManager::AddAnimator(Animator& animator)
{
	Animators.push_back(&animator);
}

int AnimationManager::AnimatorCount()
{
	return (int)Animators.size();
}


void AnimationManager::Update(double& Time, Node& node)
{
	for (int i = 0; i < node.childCount(); i++)
	{
		Object& object = node.GetNodeObject();
		Object& childObject = node.GetChildObject(i);

		if (&object)
		{
			childObject.transform.SetMatrix(object.transform.WorldMatrix);
		}
		else 
		{
			childObject.transform.SetMatrix(glm::mat4(1.0f));
		}

		glm::mat4 TranslationMatrix = UpdateTranslate(Time, childObject);
		glm::mat4 ScaleMatrix = UpdateScale(Time, childObject);
		glm::mat4 rotationMatrix = UpdateRotate(Time, childObject);

		glm::mat4 ModelMatrix = Core::Transform::MultMatrix(
			Core::Transform::MultMatrix(ScaleMatrix, rotationMatrix), TranslationMatrix);

		childObject.transform.WorldMatrix = Core::Transform::MultMatrix(ModelMatrix, childObject.transform.WorldMatrix);

		UpdateColor(Time, childObject);

		Update(Time, node.GetChildNode(i));
	}
}

glm::mat4 AnimationManager::UpdateTranslate(double& Time, Object& object)
{
	glm::mat4 outMatrix(1.0f);

	glm::vec4 value = glm::vec4(object.transform.GetLocalPosition(), 1.0f);

	if (object.animation.TranslateKeys.size() > 0)
	{
		value = object.animation.GetValueOnTime((float)Time, object.animation.TranslateKeys);
		object.transform.X = value.x;
		object.transform.Y = value.y;
		object.transform.Z = value.z;
	}
	return glm::translate(outMatrix, glm::vec3(value.x, value.y, value.z));
}

glm::mat4 AnimationManager::UpdateRotate(double& Time, Object& object)
{
	glm::vec4 value = glm::vec4(object.transform.GetLocalRotate(), 1.0f);

	if (object.animation.RotateKeys.size() > 0)
	{
		value = object.animation.GetValueOnTime((float)Time, object.animation.RotateKeys);
		object.transform.RX = value.x;
		object.transform.RY = value.y;
		object.transform.RZ = value.z;
	}

	glm::quat _quaternion(glm::vec3(glm::radians(value.x), glm::radians(value.y), glm::radians(value.z)));
	return glm::toMat4(_quaternion);
}

glm::mat4 AnimationManager::UpdateScale(double&Time, Object& object)
{
	glm::mat4 outMatrix(1.0f);

	glm::vec4 value = glm::vec4(object.transform.GetLocalScale(), 1.0f);

	if (object.animation.ScaleKeys.size() > 0)
	{
		value = object.animation.GetValueOnTime((float)Time, object.animation.ScaleKeys);
		object.transform.SX = value.x;
		object.transform.SY = value.y;
		object.transform.SZ = value.z;
	}
	return glm::scale(outMatrix, glm::vec3(value.x, value.y, value.z));
}

void AnimationManager::UpdateColor(double &Time, Object& object)
{
	if (object.animation.ColorKeys.size() > 0)
	{
		glm::vec4 outValue = object.animation.GetValueOnTime((float)Time, object.animation.ColorKeys);

		object.color.SetColor(outValue.r, outValue.g, outValue.b, outValue.a);
	}
}
