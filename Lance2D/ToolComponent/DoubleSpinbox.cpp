#ifdef ENGINE_MODE

#include "DoubleSpinbox.h"

DoubleSpinbox::DoubleSpinbox(QWidget *parent)
	: QDoubleSpinBox(parent), engineEventModel(nullptr)
{
	this->setContextMenuPolicy(Qt::CustomContextMenu);

	connect(this, SIGNAL(customContextMenuRequested(const QPoint &)),
		this, SLOT(ShowContextMenu(const QPoint &)));
}

DoubleSpinbox::~DoubleSpinbox()
{
}

void DoubleSpinbox::Initialize(EngineEventModel &model)
{
	SetEngineModel(model);
	connect(this, SIGNAL(valueChanged(double)), this, SLOT(ValueChanged(double)));
}

void DoubleSpinbox::ValueChanged(double value)
{
	if (engineEventModel->HasObject())
	{
		engineEventModel->ValueChanged(this->objectName(), value);
	}
}

void DoubleSpinbox::ShowContextMenu(const QPoint &pos)
{
	QMenu contextMenu(tr("Context menu"), this);

	QAction Action1("Set Key", this);
	connect(&Action1, SIGNAL(triggered()), this, SLOT(SetKeyAction()));
	contextMenu.addAction(&Action1);

	contextMenu.exec(mapToGlobal(pos));
}

void DoubleSpinbox::SetKeyAction()
{
	if (engineEventModel->HasObject())
	{
		engineEventModel->SetKey(this->objectName(), value());
		emit SetKey();
	}
}
#endif