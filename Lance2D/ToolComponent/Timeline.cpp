#ifdef ENGINE_MODE
#include <QtGui>
#include "Timeline.h"

const int PANEL_HEIGHT = 30;

Timeline::Timeline() : engineEventModel(nullptr)
{
	Initialize();
}

Timeline::Timeline(QWidget *parent)
	: QFrame(parent)
{
}

Timeline::Timeline(EngineEventModel &model) : engineEventModel(&model)
{
	Initialize();
}

Timeline::~Timeline()
{
}

void Timeline::Initialize()
{
	setObjectName("Timeline");
	setMinimumHeight(PANEL_HEIGHT);
	QObject::connect(this, SIGNAL(updateTimeline()), this, SLOT(repaint()));
}

void Timeline::paintEvent(QPaintEvent *e) {

	QPainter qp(this);
	drawWidget(qp);

	QFrame::paintEvent(e);
}

void Timeline::drawWidget(QPainter &qp) {

	int MaxFrame = engineEventModel->Timeline->GetFrameCount();
	int timelineGap = 1;
	int DIVISIONS = MaxFrame / timelineGap;

	QColor redColor(255, 175, 175);
	QColor timelineBG(43, 43, 43);

	int width = size().width();

	int step = (int)qRound((double)width / DIVISIONS);
	cursorWidth = step / timelineGap;

	qp.setPen(timelineBG);
	qp.setBrush(timelineBG);
	qp.drawRect(0, 0, width, PANEL_HEIGHT);

	QColor grayColor(124, 124, 124);
	qp.setPen(QPen(grayColor, 2));

	for each (auto keyFrame in engineEventModel->Frames)
	{
		int keyLeft = cursorWidth * keyFrame;
		QRectF keyRect(keyLeft, 0.0, 4, PANEL_HEIGHT);
		QPainterPath keyPath;
		keyPath.addRect(keyRect);
		qp.setBrush(QColor(255, 0, 0, 200));
		qp.fillPath(keyPath, qp.brush());
	}

	for (int i = 0; i <= DIVISIONS; i++) {
		int linePos = (i*step);
		qp.drawLine(linePos, 15, linePos, PANEL_HEIGHT);
		QFont newFont = font();
		newFont.setPointSize(7);
		setFont(newFont);

		int frame = timelineGap * i;
		QString frameStr = QString::number(frame);

		qp.drawText(linePos - 2, 8, frameStr);
	}

	cursorLeft = cursorWidth * engineEventModel->Timeline->CurrentFrame;
	QRectF cursor(cursorLeft, 0.0, cursorWidth, PANEL_HEIGHT);
	QPainterPath cursorPath;
	cursorPath.addRect(cursor);
	qp.setBrush(QColor(255, 255, 255, 60));
	qp.fillPath(cursorPath, qp.brush());

	QColor whiteColor(255, 255, 255);
	qp.setPen(QPen(whiteColor, 2));
	QString curFrameStr = QString::number(engineEventModel->Timeline->CurrentFrame);
	qp.drawText(cursorLeft, 27, curFrameStr);
}

int Timeline::GetCurrentFrame()
{
	return engineEventModel->Timeline->CurrentFrame;
}

void Timeline::mousePressEvent(QMouseEvent * event)
{
	QPoint pos = event->pos();
	engineEventModel->Timeline->IsTimelineMove = true;

	qDebug() << "mousePressEvent: " << pos.x() << ", " << pos.y();
	//emit updateTimeline();
}

void Timeline::mouseReleaseEvent(QMouseEvent * event)
{
	QPoint pos = event->pos();
	engineEventModel->Timeline->IsTimelineMove = false;
	qDebug() << "mouseReleaseEvent: " << pos.x() << ", " << pos.y();
	//emit updateTimeline();
}

void Timeline::mouseMoveEvent(QMouseEvent * event)
{
	QPoint pos = event->pos();
	if (engineEventModel->Timeline->IsTimelineMove)
	{
		engineEventModel->Timeline->CurrentFrame = pos.x() / cursorWidth;
		qDebug() << "mouseMoveEvent: " << pos.x() << " :-> " << engineEventModel->Timeline->CurrentFrame;
	}

	emit updateTimeline();
}


void Timeline::UpdateKeyFrames()
{
	engineEventModel->Frames.clear();

	if (engineEventModel->HasObject())
	{
		Object& selectedObject = engineEventModel->GetSelectObject();

		for (int i = 0; i < selectedObject.animation.TranslateKeys.size(); i++)
		{
			float frame = selectedObject.animation.TranslateKeys[i].Time;
			engineEventModel->Frames.push_back(frame);
		}

		for (int i = 0; i < selectedObject.animation.ScaleKeys.size(); i++)
		{
			float frame = selectedObject.animation.ScaleKeys[i].Time;
			engineEventModel->Frames.push_back(frame);
		}

		for (int i = 0; i < selectedObject.animation.RotateKeys.size(); i++)
		{
			float frame = selectedObject.animation.RotateKeys[i].Time;
			engineEventModel->Frames.push_back(frame);
		}

		for (int i = 0; i < selectedObject.animation.ColorKeys.size(); i++)
		{
			float frame = selectedObject.animation.ColorKeys[i].Time;
			engineEventModel->Frames.push_back(frame);
		}
	}

	repaint();
}

#endif