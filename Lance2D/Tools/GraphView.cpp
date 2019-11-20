#include "GraphView.h"
#include "NodeModel.h"


GraphView::GraphView(QWidget *parent)
	: QTreeView(parent), engineEventModel(nullptr)
{
}

GraphView::~GraphView()
{
}

void GraphView::Initialize(EngineEventModel &model)
{
	SetEngineModel(model);

	connect(this, SIGNAL(clicked(QModelIndex)), this, SLOT(SelectObject(QModelIndex)));
	connect(engineEventModel, SIGNAL(ObjectNameChanged(const QString &)), this, SLOT(ObjectNameChanged(const QString &)));
}

void GraphView::SetEngineModel(EngineEventModel &model)
{
	engineEventModel = &model;
}

void GraphView::mousePressEvent(QMouseEvent *event)
{
	QTreeView::mousePressEvent(event);

	if (!indexAt(event->pos()).isValid())
	{
		selectionModel()->clear();
	}
}

void GraphView::ObjectNameChanged(const QString &objectName)
{
	NodeModel *model = (NodeModel*)this->model();
	QModelIndex selectedIndex = model->FindIndexByName(objectName.toUtf8().constData());

	update(selectedIndex);
}

void GraphView::SelectObject(QModelIndex index)
{
	Object& prevObject = engineEventModel->GetSelectObject();
	if (&prevObject)
	{
		prevObject.SetSelected(false);
	}

	NodeModel *model = (NodeModel*)this->model();
	Node& selectedNode = model->getItem(index);

	Object& object = selectedNode.GetNodeObject();
	engineEventModel->SetObject(&object);
	engineEventModel->SetNode(&selectedNode);

	object.SetSelected(true);

	emit ObjectSelected();
}

void GraphView::SelectObject()
{
	Object& selectedObject = engineEventModel->GetSelectObject();

	NodeModel *model = (NodeModel*)this->model();
	QModelIndex selectedIndex = model->FindIndexByName(selectedObject.Name);
	this->selectionModel()->setCurrentIndex(selectedIndex, QItemSelectionModel::ClearAndSelect);

	Node& node = model->getItem(selectedIndex);
	engineEventModel->SetNode(&node);

	emit ObjectSelected();
}

