#include "Lance2D.h"
#include "Controller/GameDataManager.h"


Lance2D::Lance2D(QWidget* parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);

	engineEventModel = new EngineEventModel();
	engineEventController = new EngineEventController(*engineEventModel);

	timelinePanel = new TimelinePanel(*engineEventModel);
	inspector = new Inspector(*engineEventModel);
	ui.InspectorFrame->layout()->addWidget(inspector);

	qglView.SetEngineModel(*engineEventModel);
	ui.sceneGraph->Initialize(*engineEventModel);

	NodeModel* nodeModel = new NodeModel();
	nodeModel->RootNode = &SceneManager::GetRootNode();
	ui.sceneGraph->setModel(nodeModel);

	mainLayout.setMargin(0);
	ui.widget->setLayout(&mainLayout);
	mainLayout.addWidget(&qglView);

	QHBoxLayout* hbox = new QHBoxLayout();
	hbox->setMargin(0);
	hbox->addWidget(timelinePanel, 0);
	ui.Timeline->setLayout(hbox);

	//connect(ui.pushButton_scale, SIGNAL(clicked()), this, SLOT(Test()));

	connect(ui.Snap, SIGNAL(stateChanged(int)), engineEventModel, SLOT(SetSnapTranslate(int)));
	
	connect(ui.actionOpen, SIGNAL(triggered()), this, SLOT(openScene()));
	connect(ui.actionSave, SIGNAL(triggered()), this, SLOT(saveScene()));

	connect(ui.pushButton_duplicate, SIGNAL(clicked()), this, SLOT(clickDuplicateButton()));
	connect(ui.pushButton_delete, SIGNAL(clicked()), this, SLOT(clickDeleteButton()));

	connect(ui.pushButton_mesh, &QPushButton::clicked, [=] {clickNewObjectButton(OBJECTTYPE::MESH); });
	connect(ui.pushButton_label, &QPushButton::clicked, [=] {clickNewObjectButton(OBJECTTYPE::LABEL); });
	connect(ui.pushButton_button, &QPushButton::clicked, [=] {clickNewObjectButton(OBJECTTYPE::BUTTON); });
	connect(ui.pushButton_block1, &QPushButton::clicked, [=] {clickNewObjectButton(OBJECTTYPE::block1); });
	connect(ui.pushButton_block2, &QPushButton::clicked, [=] {clickNewObjectButton(OBJECTTYPE::block2); });
	connect(ui.pushButton_block3, &QPushButton::clicked, [=] {clickNewObjectButton(OBJECTTYPE::block3); });
	connect(ui.pushButton_block4, &QPushButton::clicked, [=] {clickNewObjectButton(OBJECTTYPE::block4); });
	connect(ui.pushButton_block5, &QPushButton::clicked, [=] {clickNewObjectButton(OBJECTTYPE::block5); });
	connect(ui.pushButton_block6, &QPushButton::clicked, [=] {clickNewObjectButton(OBJECTTYPE::block6); });
	connect(ui.pushButton_block7, &QPushButton::clicked, [=] {clickNewObjectButton(OBJECTTYPE::block7); });
	connect(ui.pushButton_plane, &QPushButton::clicked, [=] {clickNewObjectButton(OBJECTTYPE::PLANE); });
	connect(ui.pushButton_board, &QPushButton::clicked, [=] {clickNewObjectButton(OBJECTTYPE::GAMEBOARD); });


	Timeline& timeline = timelinePanel->GetTimeline();

	connect(&qglView, SIGNAL(ObjectSelected()), ui.sceneGraph, SLOT(SelectObject()));
	connect(ui.sceneGraph, SIGNAL(ObjectSelected()), inspector, SLOT(UpdateInspector()));
	connect(&timeline, SIGNAL(updateTimeline()), inspector, SLOT(UpdateInspector()));

	connect(inspector, SIGNAL(SetKey()), &timeline, SLOT(UpdateKeyFrames()));
	connect(&qglView, SIGNAL(updateTimeline()), &timeline, SLOT(UpdateKeyFrames()));
	connect(ui.sceneGraph, SIGNAL(ObjectSelected()), &timeline, SLOT(UpdateKeyFrames()));

	qglView.setFocus();
}

void Lance2D::Test()
{
	GameDataManager::SaveGameData();
	std::string score = GameDataManager::GetTopScore();
	qDebug() << score.c_str();
}

void Lance2D::clickPrint()
{
}

Lance2D::~Lance2D()
{
	delete engineEventController;
	delete engineEventModel;
}

void Lance2D::saveScene()
{
	QString filename = QFileDialog::getSaveFileName(this,
		tr("Save Scene"), "",
		tr("Scene (*.txt);;All Files (*)"));

	if (filename.size() > 0)
	{
		engineEventController->SaveScene(filename.toUtf8().constData(), SceneManager::GetRootNode());
	}
}

void Lance2D::openScene()
{
	QString filename = QFileDialog::getOpenFileName(this, tr("Open Scene"), "",
		tr("Scene (*.txt);;All Files (*)"));

	if (filename.size() > 0)
	{
		engineEventController->OpenScene(filename.toUtf8().constData());
		ui.sceneGraph->reset();
	}
}


void Lance2D::clickNewObjectButton(OBJECTTYPE obejctType)
{
	Object& object = engineEventController->NewObjectEvent(obejctType);

	Object& selectedObject = engineEventModel->GetSelectObject();
	if (&selectedObject)
	{
		glm::mat4 parentInvMatrix = selectedObject.transform.GetInverseMatrix();
		glm::mat4 localMatrix = parentInvMatrix + object.transform.WorldMatrix;
		glm::vec3 localPos = Core::Transform::GetPositionByMatrix(localMatrix);

		object.transform.SetMatrix(selectedObject.transform.WorldMatrix);
		object.transform.SetTranslate(localPos.x, localPos.y, localPos.z);
	}

	///////
	QModelIndex currentIndex = ui.sceneGraph->selectionModel()->currentIndex();
	NodeModel* model = (NodeModel*)ui.sceneGraph->model();
	if (!model->insertRow(0, currentIndex))
		return;

	int rows = model->rowCount(currentIndex);
	QModelIndex child = model->index(rows - 1, 0, currentIndex);
	model->setData(child, object);

	//ui.sceneGraph->selectionModel()->setCurrentIndex(child, QItemSelectionModel::ClearAndSelect);
	//SelectObject(child);
}

void Lance2D::clickDuplicateButton()
{
	Object& selectedObject = engineEventModel->GetSelectObject();
	if (&selectedObject)
	{
		Object& newObject = engineEventController->CopyObjectEvent(selectedObject);

		NodeModel* model = (NodeModel*)ui.sceneGraph->model();
		Node& selectedNode = SceneManager::GetSceneRoot().FindNode(selectedObject);
		QModelIndex parentIndex;
		Object& parentObject = selectedNode.parent().GetNodeObject();
		if (&parentObject)
		{
			parentIndex = model->FindIndexByName(parentObject.Name);
		}

		if (!model->insertEmptyRow(0, parentIndex))
			return;

		//int rows = model->rowCount(parentIndex);
		//QModelIndex child = model->index(rows - 1, 0, parentIndex);

		//ui.sceneGraph->selectionModel()->setCurrentIndex(child, QItemSelectionModel::ClearAndSelect);
		//SelectObject(child);
	}
}

void Lance2D::clickDeleteButton()
{
	if (engineEventModel->HasObject())
	{
		//engineEventController->DeleteObjectEvent()
		NodeModel* model = (NodeModel*)ui.sceneGraph->model();
		QModelIndex index = model->FindIndexByName(engineEventModel->GetSelectObject().Name);
		model->removeRow(index.row(), index.parent());

		Node& rootNode = *SceneManager::GetSceneRoot().RootNode;

		engineEventModel->SetObject(nullptr);
	}
}

