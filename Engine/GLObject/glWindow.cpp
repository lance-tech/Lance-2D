#include "glWindow.h"

glWindow::glWindow() :
	ScreenWidth(0), ScreenHeight(0),
	Projection(1.0f), View(1.0f), VP(1.0f)
#ifdef GAME_MODE
	, window(nullptr), 
	OnButtonPrees(false), OnKeyPrees(false), KeyType(0)
#elif ENGINE_MODE
#endif
{
	initGL();
	Initialize();
}

glWindow::glWindow(int screenWidth, int screenHeight) : glWindow()
{
}

glWindow::~glWindow()
{
	SceneManager* instance = SceneManager::GetInstance();
	if (instance)
	{
		delete instance;
	}

#ifdef GAME_MODE
	glfwTerminate();
#elif ENGINE_MODE
#endif
}

void glWindow::Initialize()
{
}

GLvoid glWindow::initGLEW()
{
	// Set this to true so GLEW knows to use a modern approach to retrieving function pointers and extensions
	glewExperimental = GL_TRUE;
	// Initialize GLEW to setup the OpenGL Function pointers
	if (GLEW_OK != glewInit())
	{
		std::cout << "Failed to initialize GLEW" << std::endl;
		return;
	}
}

GLvoid glWindow::initView()
{
	glViewport(0, 0, ScreenWidth, ScreenHeight);

	glClearColor(0.0f, 0.1f, 0.3f, 0.0f);
	
	glEnable(GL_MULTISAMPLE);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glEnable(GL_CULL_FACE);

	glEnable(GL_ALPHA_TEST);
	glAlphaFunc(GL_GREATER, 0.0);
	

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Setup Camera
	Projection = glm::ortho(
		WINDOW_LEFT,
		WINDOW_RIGHT,
		WINDOW_BOTTOM,
		WINDOW_TOP,
		-100.0f, 100.0f);
	View = glm::lookAt(
		glm::vec3(0, 0, 1), // World Space
		glm::vec3(0, 0, 0), // looks at the origin
		glm::vec3(0, 1, 0)  // Head is up (set to 0,-1,0 to look upside-down)
	);

	//Projection = glm::perspective(glm::radians(45.0f), 4.0f / 3.0f, 0.1f, 100.0f);
	//View = glm::lookAt(
	//	glm::vec3(0, 10, 10), // World Space
	//	glm::vec3(0, 0, 0), // looks at the origin
	//	glm::vec3(0, 1, 0)  // Head is up (set to 0,-1,0 to look upside-down)
	//);

	VP = Projection * View;
}

GLvoid glWindow::initGL()
{
#ifdef GAME_MODE
	// Init GLFW
	glfwInit();

	// Set all the required options for GLFW
	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_RESIZABLE, GLU_FALSE);

	// Create a GLFWwindow object that we can use for GLFW's functions
	window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Game Window", nullptr, nullptr);

	glfwGetFramebufferSize(window, &ScreenWidth, &ScreenHeight);

	if (nullptr == window)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return;
	}

	glfwMakeContextCurrent(window);

	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
#elif ENGINE_MODE
#endif

	initGLEW();
	initView();
}

void glWindow::Start()
{
#ifdef GAME_MODE
	while (!glfwWindowShouldClose(window))
	{
		KeyPressEvent();
		KeyReleaseEvent();
		mousePressEvent();
		mouseMoveEvent();
		mouseReleaseEvent();

		// Check if any events have been activiated (key pressed, mouse moved etc.) and call corresponding response functions
		glfwPollEvents();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		double time = glfwGetTime();
		Update(time);

		// Swap the screen buffers
		glfwSwapBuffers(window);
	}
#elif ENGINE_MODE
#endif
}

void glWindow::Update(double &deltaTime)
{
	SceneManager::updateCurrentScene(VP, deltaTime);
}

void glWindow::KeyPressEvent(int event)
{
	GameScene* currentScene = SceneManager::GetCurrentScene();
#ifdef GAME_MODE

	if (OnKeyPrees)
	{
		return;
	}

	int state = glfwGetKey(window, KEY_UP);
	if (state == GLFW_PRESS && KeyType == 0)
	{
		OnKeyPrees = true;
		KeyType = KEY_UP;
		currentScene->OnKeyPressed(KEY_UP);
	}

	state = glfwGetKey(window, KEY_DOWN);
	if (state == GLFW_PRESS && KeyType == 0)
	{
		OnKeyPrees = true;
		KeyType = KEY_DOWN;
		currentScene->OnKeyPressed(KEY_DOWN);
	}

	state = glfwGetKey(window, KEY_LEFT);
	if (state == GLFW_PRESS && KeyType == 0)
	{
		OnKeyPrees = true;
		KeyType = KEY_LEFT;
		currentScene->OnKeyPressed(KEY_LEFT);
	}

	state = glfwGetKey(window, KEY_RIGHT);
	if (state == GLFW_PRESS && KeyType == 0)
	{
		OnKeyPrees = true;
		KeyType = KEY_RIGHT;
		currentScene->OnKeyPressed(KEY_RIGHT);
	}

	state = glfwGetKey(window, KEY_SPACE);
	if (state == GLFW_PRESS && KeyType == 0)
	{
		OnKeyPrees = true;
		KeyType = KEY_SPACE;
		currentScene->OnKeyPressed(KEY_SPACE);
	}
#elif ENGINE_MODE
	currentScene->OnKeyPressed(event);
#endif
}

void glWindow::KeyReleaseEvent(int event)
{
	GameScene* currentScene = SceneManager::GetCurrentScene();
#ifdef GAME_MODE

	if (!OnKeyPrees)
	{
		return;
	}

	int state = glfwGetKey(window, KEY_UP);
	if (state == GLFW_RELEASE && KeyType == KEY_UP)
	{
		OnKeyPrees = false;
		KeyType = 0;
		currentScene->OnKeyReleased(KEY_UP);
	} 

	state = glfwGetKey(window, KEY_DOWN);
	if (state == GLFW_RELEASE && KeyType == KEY_DOWN)
	{
		OnKeyPrees = false;
		KeyType = 0;
		currentScene->OnKeyReleased(KEY_DOWN);
	}

	state = glfwGetKey(window, KEY_LEFT);
	if (state == GLFW_RELEASE && KeyType == KEY_LEFT)
	{
		OnKeyPrees = false;
		KeyType = 0;
		currentScene->OnKeyReleased(KEY_LEFT);
	}

	state = glfwGetKey(window, KEY_RIGHT);
	if (state == GLFW_RELEASE && KeyType == KEY_RIGHT)
	{
		OnKeyPrees = false;
		KeyType = 0;
		currentScene->OnKeyReleased(KEY_RIGHT);
	}

	state = glfwGetKey(window, KEY_SPACE);
	if (state == GLFW_RELEASE && KeyType == KEY_SPACE)
	{
		OnKeyPrees = false;
		KeyType = 0;
		currentScene->OnKeyReleased(KEY_SPACE);
	}
#elif ENGINE_MODE
	currentScene->OnKeyPressed(event);
#endif
}

void glWindow::mouseMoveEvent(double x, double y)
{
	GameScene* currentScene = SceneManager::GetCurrentScene();
#ifdef GAME_MODE
	double _x, _y;
	glfwGetCursorPos(window, &_x, &_y);

	double screenPosX = (((WINDOW_LEFT * -1.f) + WINDOW_RIGHT) * _x) / ScreenWidth;
	double screenPosY = ((WINDOW_TOP + (WINDOW_BOTTOM * -1.f)) * _y) / ScreenHeight;

	currentScene->OnMouseMove(screenPosX, screenPosY);
#elif ENGINE_MODE
	currentScene->OnMouseMove(x, y);
#endif
}

void glWindow::mousePressEvent(int event, double x, double y)
{
	GameScene* currentScene = SceneManager::GetCurrentScene();
#ifdef GAME_MODE
	
	if (OnButtonPrees)
	{
		return;
	}

	double _x, _y;
	glfwGetCursorPos(window, &_x, &_y);
	double screenPosX = (((WINDOW_LEFT * -1.f) + WINDOW_RIGHT) * _x) / WINDOW_WIDTH;
	double screenPosY = ((WINDOW_TOP + (WINDOW_BOTTOM * -1.f)) * _y) / WINDOW_HEIGHT;

	int state = glfwGetMouseButton(window, L_BUTTON);
	if (state == GLFW_PRESS)
	{
		OnButtonPrees = true;
		currentScene->OnMousePressed(L_BUTTON, screenPosX, screenPosY);
	}
#elif ENGINE_MODE
	currentScene->OnMousePressed(event, x, y);
#endif
}

void glWindow::mouseReleaseEvent(int event, double x, double y)
{
	GameScene* currentScene = SceneManager::GetCurrentScene();
#ifdef GAME_MODE
	if (!OnButtonPrees)
	{
		return;
	}

	double input_x, input_y;
	glfwGetCursorPos(window, &input_x, &input_y);

	double windowWidth = ((double)WINDOW_LEFT * -1.) + (double)WINDOW_RIGHT;
	double windowHeight = (double)WINDOW_TOP + ((double)WINDOW_BOTTOM * -1.);

	double screenPosX = (windowWidth * input_x) / ScreenWidth;
	double screenPosY = (windowHeight * input_y) / ScreenHeight;

	int state = glfwGetMouseButton(window, L_BUTTON);
	if (state == GLFW_RELEASE)
	{
		OnButtonPrees = false;
		currentScene->OnMouseReleased(L_BUTTON, screenPosX, screenPosY);
	}
#elif ENGINE_MODE
	currentScene->OnMouseReleased(event, x, y);
#endif
}