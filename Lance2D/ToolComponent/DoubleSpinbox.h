#ifdef ENGINE_MODE

#pragma once

#include <QWidget>
#include <QDoubleSpinBox>
#include <QMenu>

#include "../Tools/EngineEventModel.h"


class DoubleSpinbox : public QDoubleSpinBox
{
	Q_OBJECT

public:
	DoubleSpinbox(QWidget *parent = Q_NULLPTR);
	~DoubleSpinbox();
	void Initialize(EngineEventModel &model);
	void SetEngineModel(EngineEventModel &model)
	{
		engineEventModel = &model;
	}

private:
	EngineEventModel *engineEventModel;

private slots:
	void ShowContextMenu(const QPoint &pos);
	void SetKeyAction();
	void ValueChanged(double value);

signals:
	void SetKey();
};



#endif