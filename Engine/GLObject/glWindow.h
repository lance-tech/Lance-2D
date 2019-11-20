#ifndef GLWINDOW_H
#define GLWINDOW_H

#include "../Engine.h"

#include <GL/glew.h>

#ifdef GAME_MODE
#include <GLFW/glfw3.h>
#endif

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "../Controller/AnimationManager.h"
#include "../Controller/SceneManager.h"
#include "../Controller/MaterialManager.h"
#include "../Core/Control.h"
#include "../Core/ModelLoader.h"
#include "Object.h"


class glWindow
{
public:
	glWindow();
	glWindow(int screenWidth, int screenHeight);
	virtual ~glWindow();

	void Start();
	
protected:
	GLvoid initGLEW();
	GLvoid initView();
	virtual GLvoid initGL();
	virtual void Initialize();
	virtual void Update(double &deltaTime);

	void KeyPressEvent(int event=0);
	void KeyReleaseEvent(int event = 0);
	void mouseMoveEvent(double x=0, double y=0);
	void mousePressEvent(int event=0, double x = 0, double y = 0);
	void mouseReleaseEvent(int event=0, double x = 0, double y = 0);

	int ScreenWidth, ScreenHeight;
	glm::mat4 Projection;
	glm::mat4 View;
	glm::mat4 VP;

#ifdef GAME_MODE
	GLFWwindow* window;
	bool OnButtonPrees;
	bool OnKeyPrees;
	int KeyType;
#elif ENGINE_MODE
#endif
};

#endif