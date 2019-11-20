#pragma once

#include "../GLObject/Object.h"

class Collision
{
public:
	Collision();
	~Collision();

	static bool CollisionWithPoint(Object& object, double& x, double& y);
};

