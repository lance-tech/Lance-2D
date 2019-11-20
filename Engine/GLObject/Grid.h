#pragma once

#include "../Engine.h"
#include "glRect2D.h"

// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>


class Grid : public glRect2D
{
public:
	Grid();
	~Grid();

	void Draw(glm::mat4 &VP, double& time) override;
};

