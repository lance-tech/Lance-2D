#include "QGLWindow.h"

QGLWindow::QGLWindow(QWidget *parent)
	: QWidget(parent), glWindow(this->width(), this->height()),
	engineEventModel(nullptr)
{
	initGL();
	Initialize();
}


QGLWindow::~QGLWindow()
{
	if (hrc)
	{
		wglMakeCurrent(NULL, NULL);
		wglDeleteContext(hrc);
		hrc = NULL;
		ReleaseDC((HWND)winId(), hdc);
	}
}

void QGLWindow::Initialize()
{
	EmptyScene* emptyScene = new EmptyScene();
	SceneManager::AddGameScene(*emptyScene);
	SceneManager::ResetScene();
	emptyScene->timeline.IsPlay = false;
}

void QGLWindow::SetEngineModel(EngineEventModel &model)
{
	engineEventModel = &model;
	engineEventModel->SetTimeModel(SceneManager::GetCurrentScene()->timeline);
}

void QGLWindow::paintEvent(QPaintEvent* paintEvent)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	double time = GetTickCount64();
	Update(time);
	SwapBuffers(hdc);
}


void QGLWindow::Update(double &deltaTime)
{
	if (engineEventModel->Timeline->IsPlay)
	{
		emit updateTimeline();
	}

	glWindow::Update(deltaTime);
}


void QGLWindow::resizeEvent(QResizeEvent* resizeEvent)
{
	QWidget::resizeEvent(resizeEvent);
	auto size = resizeEvent->size();
	ScreenWidth = size.width();
	ScreenHeight = size.height();

	if ((ScreenWidth < 0) || (ScreenHeight < 0))
		return;

	if (glViewport != NULL) {
		glViewport(0, 0, ScreenWidth, ScreenHeight);
	}
}

GLvoid QGLWindow::initGL()
{
	setAttribute(Qt::WA_NativeWindow);
	setAttribute(Qt::WA_PaintOnScreen);
	setAttribute(Qt::WA_NoSystemBackground);

	PIXELFORMATDESCRIPTOR pfd = {
		sizeof(PIXELFORMATDESCRIPTOR), 
		1,
		PFD_DRAW_TO_WINDOW | 
		PFD_SUPPORT_OPENGL | 
		PFD_DOUBLEBUFFER,
		PFD_TYPE_RGBA, 
		32, 
		0, 0, 0, 0, 0, 0,
		0, 0, 
		0, 0, 0, 0, 0,
		24, 
		8, 
		0,
		PFD_MAIN_PLANE, 
		0, 
		0, 0, 0 
	};

	hdc = GetDC((HWND)winId());

	int windowPixelFormat = ChoosePixelFormat(hdc, &pfd);
	SetPixelFormat(hdc, windowPixelFormat, &pfd);

	hrc = wglCreateContext(hdc);
	wglMakeCurrent(hdc, hrc);

	glWindow::initGL();
	
	connect(&updateTimer, SIGNAL(timeout()), this, SLOT(update()));
	updateTimer.start(0);
	
}

void QGLWindow::keyPressEvent(QKeyEvent *event)
{
	glWindow::KeyPressEvent(event->key());
}

void QGLWindow::mousePressEvent(QMouseEvent *event)
{
	QPoint pos = event->pos();
	float screenPosX = (((WINDOW_LEFT * -1.f) + WINDOW_RIGHT) * pos.x()) / ScreenWidth;
	float screenPosY = ((WINDOW_TOP + (WINDOW_BOTTOM * -1.f)) * pos.y()) / ScreenHeight;

	glWindow::mousePressEvent(event->button(), screenPosX, screenPosY);

	engineEventModel->InputPosX = screenPosX;
	engineEventModel->InputPosY = screenPosY;
	engineEventModel->SetObject(&SceneManager::GetSceneRoot().GetSelectedObject());
	if (engineEventModel->HasObject())
	{
		if (engineEventModel->GetSelectObject().IsSelected)
		{
			emit ObjectSelected();
			emit updateTimeline();
			engineEventModel->ChangedObjectTransform();
		}
	}
	this->setFocus();
}

void QGLWindow::mouseMoveEvent(QMouseEvent *event)
{
	QPoint pos = event->pos();
	float screenPosX = (((WINDOW_LEFT * -1.f) + WINDOW_RIGHT) * pos.x()) / ScreenWidth;
	float screenPosY = ((WINDOW_TOP + (WINDOW_BOTTOM * -1.f)) * pos.y()) / ScreenHeight;

	glWindow::mouseMoveEvent(screenPosX, screenPosY);
	
	engineEventModel->InputPosX = screenPosX;
	engineEventModel->InputPosY = screenPosY;

	if (engineEventModel->HasObject())
	{
		if (engineEventModel->GetSelectObject().IsSelected)
		{
			emit ObjectSelected();
			engineEventModel->ChangedObjectTransform();
		}
	}
}

void QGLWindow::mouseReleaseEvent(QMouseEvent *event)
{
	QPoint pos = event->pos();
	float screenPosX = (((WINDOW_LEFT * -1.f) + WINDOW_RIGHT) * pos.x()) / ScreenWidth;
	float screenPosY = ((WINDOW_TOP + (WINDOW_BOTTOM * -1.f)) * pos.y()) / ScreenHeight;

	glWindow::mouseReleaseEvent(event->button(), screenPosX, screenPosY);

	engineEventModel->InputPosX = screenPosX;
	engineEventModel->InputPosY = screenPosY;
}

