#pragma once
#include <vector>
#include "../GLObject/Object.h"
#include "../Core/Node.h"
#include "../Core/Animator.h"


class AnimationManager
{
public:
	AnimationManager();
	~AnimationManager();

	void Update(double& Time, Node& node);
	void UpdateAnimators();
	bool IsAnimatorsPlay;
	int AnimatorCount();
	void AddAnimator(Animator& animator);
	void PlayAnimators();

private:
	glm::mat4 UpdateTranslate(double &Time, Object& object);
	glm::mat4 UpdateRotate(double&Time, Object& object);
	glm::mat4 UpdateScale(double&Time, Object& object);
	void UpdateColor(double&Time, Object& object);

	std::vector<Animator*> Animators;
};

