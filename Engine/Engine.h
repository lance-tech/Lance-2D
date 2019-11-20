#pragma once

enum class OBJECTTYPE : unsigned int
{
	block1 = 0,
	block2,
	block3,
	block4,
	block5,
	block6,
	block7,
	block0,
	PLANE,
	GAMEBOARD,
	BUTTON,
	LABEL,
	MESH
};


// Window dimensions
const int WINDOW_WIDTH = 1024;
const int WINDOW_HEIGHT = 768;
const float WINDOW_LEFT = -20.f;
const float WINDOW_RIGHT = 20.f;
//const float WINDOW_LEFT = -20.0f * (WINDOW_WIDTH / WINDOW_HEIGHT);
//const float WINDOW_RIGHT = 20.0f * (WINDOW_WIDTH / WINDOW_HEIGHT);
const float WINDOW_BOTTOM = -20.f;
const float WINDOW_TOP = 20.f;
