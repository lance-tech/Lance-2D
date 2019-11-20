#pragma once

#include <QWidget>
#include <QFrame>
#include <QBoxLayout>
#include "ui_Timeline.h"
#include <vector>

#include "EngineEventModel.h"
#include "../ToolComponent/Timeline.h"


class TimelinePanel : public QFrame
{
	Q_OBJECT

public:
	TimelinePanel();
	TimelinePanel(QWidget *parent = Q_NULLPTR);
	explicit TimelinePanel(EngineEventModel &model);
	~TimelinePanel();

	void Initialize();

	EngineEventModel *engineEventModel;

	Timeline& GetTimeline();
private:
	Ui::TimelineClass ui;
	Timeline *TimelineWidget;

public slots:
	void clickAnimPlay();
	void valueChangedSF(int value);
	void valueChangedEF(int value);
	void UpdateTimelinePanel();

//signals:
};

