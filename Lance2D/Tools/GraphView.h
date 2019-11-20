#pragma once

#include <QWidget>
#include <QTreeView>
#include <QMouseEvent>


#include "EngineEventModel.h"


class GraphView : public QTreeView
{
	Q_OBJECT

public:
	GraphView(QWidget *parent);
	~GraphView();
	void Initialize(EngineEventModel &model);
	void SetEngineModel(EngineEventModel &model);

	virtual void mousePressEvent(QMouseEvent *event) override;

private:
	EngineEventModel *engineEventModel;

private slots:
	void ObjectNameChanged(const QString &objectName);
	void SelectObject(QModelIndex index);
	void SelectObject();

signals:
	void ObjectSelected();
};
