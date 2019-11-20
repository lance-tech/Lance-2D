#pragma once

#include <QWidget>
#include <QFrame>
#include "ui_Inspector.h"
#include "EngineEventModel.h"


class Inspector : public QFrame
{
	Q_OBJECT

public:
	Inspector(QWidget *parent = Q_NULLPTR);
	explicit Inspector(EngineEventModel &model);
	~Inspector();
	void Initialize();
	void SetEngineModel(EngineEventModel &model);
	void SetParent(QWidget *parent);

private:
	Ui::InspectorClass ui;

	EngineEventModel *engineEventModel;
	QWidget *ParentWidget;

public slots:
	void SetObjectName(const QString& text);
	void SetVertexShader();
	void SetFragmentShader();
	void ClickCompileShader();
	void ClickOpenTexture();
	void ClickOpenMesh();
	void ClickSetKey();
	void WidgetTextChanged(const QString &text);
	void FontSizeChanged(double size);
	void UpdateInspector();

signals:
	void SetKey();
};


