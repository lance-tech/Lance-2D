#ifdef ENGINE_MODE
#pragma once

#include <QWidget>
#include <QFrame>
#include <QBoxLayout>
#include <vector>

#include "../Tools/EngineEventModel.h"


class Timeline : public QFrame
{
	Q_OBJECT

public:
	Timeline();
	Timeline(QWidget *parent = Q_NULLPTR);
	explicit Timeline(EngineEventModel &model);
	~Timeline();

	void Initialize();
	int GetCurrentFrame();

	EngineEventModel *engineEventModel;

protected:
	void paintEvent(QPaintEvent *e);
	void drawWidget(QPainter &qp);

	virtual void mousePressEvent(QMouseEvent *event) override;
	virtual void mouseReleaseEvent(QMouseEvent *event) override;
	virtual void mouseMoveEvent(QMouseEvent *event) override;

private:
	int DISTANCE = 19;
	int LINE_WIDTH = 5;
	float FULL_CAPACITY = 700.f;
	float MAX_CAPACITY = 750.f;

	int cursorWidth = 10;
	int cursorHeight = 30;
	int cursorLeft = 0;

public slots:
	void UpdateKeyFrames();

signals:
	void updateTimeline();
};
#endif