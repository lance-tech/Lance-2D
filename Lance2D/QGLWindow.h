#pragma once

#include <GLObject/glWindow.h>
#include <Core/Control.h>
#include <Scene/EmptyScene.h>

#include <Qt>
#include <QThread>
#include <QTimer>
#include <QKeyEvent>
#include <QMouseEvent>
#include <QResizeEvent>
#include <QWidget>
#include <qopenglwidget.h>

#include "Tools/EngineEventModel.h"


class QGLWindow : public QWidget, public glWindow
{
	Q_OBJECT
public:
	explicit QGLWindow(QWidget *parent = Q_NULLPTR);
	virtual ~QGLWindow() override;

	virtual QPaintEngine* paintEngine() const override
	{
		return nullptr;
	}

	void SetEngineModel(EngineEventModel &model);
private:
	GLvoid initGL() override;
	void Initialize() override;

	HDC hdc;
	HGLRC hrc;
	QTimer updateTimer;
	EngineEventModel* engineEventModel;

protected:
	virtual void Update(double &deltaTime) override;

	virtual void paintEvent(QPaintEvent* paintEvent) override;
	virtual void resizeEvent(QResizeEvent* resizeEvent) override;
	virtual void keyPressEvent(QKeyEvent *event) override;
	virtual void mousePressEvent(QMouseEvent *event) override;
	virtual void mouseReleaseEvent(QMouseEvent *event) override;
	virtual void mouseMoveEvent(QMouseEvent *event) override;
	
signals:
	void updateTimeline();
	void ObjectSelected();
};

