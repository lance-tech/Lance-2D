#include "Collision.h"

Collision::Collision()
{
}

Collision::~Collision()
{
}

bool Collision::CollisionWithPoint(Object& object, double& x, double& y)
{
	bool res = false;
	glm::vec3 screenPos = object.transform.GetScreenPosition();
	glm::vec3 scale = object.transform.GetLocalScale();
	
	double left = (scale.x / 2.) * -1.;
	double right = scale.x / 2.;
	double top = scale.y / 2.;
	double bottom = (scale.y / 2.) * -1.;

	if (x > screenPos.x + left && x < screenPos.x + right &&
		y > screenPos.y + bottom && y < screenPos.y + top)
	{
		res = true;
	}
	return res;
}
