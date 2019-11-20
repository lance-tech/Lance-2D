#include <QtGui>
#include "TimelinePanel.h"


TimelinePanel::TimelinePanel() : engineEventModel(nullptr)
{
	ui.setupUi(this);
	Initialize();
}

TimelinePanel::TimelinePanel(QWidget *parent)
	: QFrame(parent)
{
}

TimelinePanel::TimelinePanel(EngineEventModel &model) : engineEventModel(&model)
{
	ui.setupUi(this);
	Initialize();
}

TimelinePanel::~TimelinePanel()
{
}

void TimelinePanel::Initialize()
{
	

	TimelineWidget = new Timeline(*engineEventModel);

	QHBoxLayout *hbox = new QHBoxLayout();
	hbox->setMargin(0);
	hbox->addWidget(TimelineWidget, 0);
	ui.timeline_frame->setLayout(hbox);

	connect(engineEventModel, SIGNAL(UpdateTimeInfo()), this, SLOT(UpdateTimelinePanel()));
	connect(ui.animPlayButton, SIGNAL(clicked()), this, SLOT(clickAnimPlay()));
	connect(ui.StartFrame, SIGNAL(valueChanged(int)), this, SLOT(valueChangedSF(int)));
	connect(ui.EndFrame, SIGNAL(valueChanged(int)), this, SLOT(valueChangedEF(int)));
}

void TimelinePanel::UpdateTimelinePanel()
{
	ui.StartFrame->setValue(engineEventModel->Timeline->StartFrame);
	ui.EndFrame->setValue(engineEventModel->Timeline->EndFrame);
	TimelineWidget->UpdateKeyFrames();
}

void TimelinePanel::clickAnimPlay()
{
	if (engineEventModel->Timeline->IsPlay)
	{
		engineEventModel->Timeline->IsPlay = false;
		ui.animPlayButton->setText("Play");
	}
	else
	{
		engineEventModel->Timeline->IsPlay = true;
		ui.animPlayButton->setText("Pause");
	}
}

void TimelinePanel::valueChangedSF(int value)
{
	engineEventModel->Timeline->StartFrame = value;
	TimelineWidget->repaint();
}

void TimelinePanel::valueChangedEF(int value)
{
	engineEventModel->Timeline->EndFrame = value;
	TimelineWidget->repaint();
}

Timeline& TimelinePanel::GetTimeline()
{
	return *TimelineWidget;
}


