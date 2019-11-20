#pragma once

#include <QtWidgets/QMainWindow>
#include <QFileDialog>
#include <QKeyEvent>
#include "ui_Lance2D.h"

#include "QGLWindow.h"

#include <Controller/SceneManager.h>
#include "Controller/EngineEventController.h"

#include "Tools/TimelinePanel.h"
#include "ToolComponent/Timeline.h"

#include "Tools/EngineEventModel.h"
#include "Tools/NodeModel.h"
#include "Tools/Inspector.h"


class Lance2D : public QMainWindow
{
	Q_OBJECT

public:
	Lance2D(QWidget* parent = Q_NULLPTR);
	~Lance2D();

private:
	Ui::Lance2DClass ui;

	QVBoxLayout mainLayout;
	QGLWindow qglView;
	TimelinePanel* timelinePanel;
	Inspector* inspector;
	EngineEventController* engineEventController;
	EngineEventModel* engineEventModel;

private slots:
	void clickPrint();
	void Test();

	void clickDuplicateButton();
	void clickDeleteButton();
	void clickNewObjectButton(OBJECTTYPE wType);

	void saveScene();
	void openScene();
};

