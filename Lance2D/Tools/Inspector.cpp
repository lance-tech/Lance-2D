#include "Inspector.h"
#include <QFileDialog>
#include <GLObject/glWidget.h>
#include <GLObject/glMesh.h>


Inspector::Inspector(QWidget *parent)
	: QFrame(parent), engineEventModel(nullptr)
{
	ui.setupUi(this);
}

Inspector::~Inspector()
{
}

Inspector::Inspector(EngineEventModel &model) : engineEventModel(&model)
{
	ui.setupUi(this);
	Initialize();
}

void Inspector::Initialize()
{
	ui.cr->Initialize(*engineEventModel);
	ui.cg->Initialize(*engineEventModel);
	ui.cb->Initialize(*engineEventModel);
	ui.ca->Initialize(*engineEventModel);

	ui.fcr->Initialize(*engineEventModel);
	ui.fcg->Initialize(*engineEventModel);
	ui.fcb->Initialize(*engineEventModel);
	ui.fca->Initialize(*engineEventModel);
	ui.specular->Initialize(*engineEventModel);

	ui.tx->Initialize(*engineEventModel);
	ui.ty->Initialize(*engineEventModel);
	ui.tz->Initialize(*engineEventModel);

	ui.sx->Initialize(*engineEventModel);
	ui.sy->Initialize(*engineEventModel);
	ui.sz->Initialize(*engineEventModel);

	ui.rx->Initialize(*engineEventModel);
	ui.ry->Initialize(*engineEventModel);
	ui.rz->Initialize(*engineEventModel);

	QObject::connect(ui.objectName, SIGNAL(textChanged(const QString &)), this, SLOT(SetObjectName(const QString &)));
	connect(ui.openVShaderFile, SIGNAL(clicked()), this, SLOT(SetVertexShader()));
	connect(ui.openFShaderFile, SIGNAL(clicked()), this, SLOT(SetFragmentShader()));
	connect(ui.ShaderCompile, SIGNAL(clicked()), this, SLOT(ClickCompileShader()));
	connect(ui.openTexFile, SIGNAL(clicked()), this, SLOT(ClickOpenTexture()));
	connect(ui.openMeshFile, SIGNAL(clicked()), this, SLOT(ClickOpenMesh()));

	connect(ui.tx, SIGNAL(SetKey()), this, SLOT(ClickSetKey()));
	connect(ui.ty, SIGNAL(SetKey()), this, SLOT(ClickSetKey()));
	connect(ui.rx, SIGNAL(SetKey()), this, SLOT(ClickSetKey()));
	connect(ui.ry, SIGNAL(SetKey()), this, SLOT(ClickSetKey()));
	connect(ui.rz, SIGNAL(SetKey()), this, SLOT(ClickSetKey()));
	connect(ui.sx, SIGNAL(SetKey()), this, SLOT(ClickSetKey()));
	connect(ui.sy, SIGNAL(SetKey()), this, SLOT(ClickSetKey()));
	connect(ui.sz, SIGNAL(SetKey()), this, SLOT(ClickSetKey()));
	connect(ui.cr, SIGNAL(SetKey()), this, SLOT(ClickSetKey()));
	connect(ui.cg, SIGNAL(SetKey()), this, SLOT(ClickSetKey()));
	connect(ui.cb, SIGNAL(SetKey()), this, SLOT(ClickSetKey()));
	connect(ui.ca, SIGNAL(SetKey()), this, SLOT(ClickSetKey()));

	connect(ui.text, SIGNAL(textChanged(const QString &)), this, SLOT(WidgetTextChanged(const QString &)));
	connect(ui.fontsize, SIGNAL(valueChanged(double)), this, SLOT(FontSizeChanged(double)));
}

void Inspector::SetEngineModel(EngineEventModel &model)
{
	engineEventModel = &model;
}

void Inspector::SetParent(QWidget * parent)
{
	ParentWidget = parent;
}

void Inspector::SetObjectName(const QString& text)
{
	if (!engineEventModel)
		return;

	engineEventModel->SetObjectName(text);
}

void Inspector::SetVertexShader()
{
	if (!engineEventModel)
		return;

	QString filename = QFileDialog::getOpenFileName(this, tr("Select Shader File"), "", 
		tr("Vertex Shader (*.vert);;All Shader Files (*.vert)"));
	
	if (filename.size() > 0)
	{
		int index = filename.indexOf("/Assets/");
		QString subString = filename.mid(index + 1);

		engineEventModel->SetShader(subString);
		
		ui.VshaderPath->setText(subString);
	}
}


void Inspector::SetFragmentShader()
{
	if (!engineEventModel)
		return;

	QString filename = QFileDialog::getOpenFileName(this, tr("Select Shader File"), "",
		tr("Fragment Shader (*.frag);;All Shader Files (*.frag)"));

	if (filename.size() > 0)
	{
		int index = filename.indexOf("/Assets/");
		QString subString = filename.mid(index + 1);

		engineEventModel->SetShader(subString);
		ui.FshaderPath->setText(subString);
	}
}

void Inspector::ClickCompileShader()
{
	if (!engineEventModel)
		return;

	Object& selectedObject = engineEventModel->GetSelectObject();
	if (&selectedObject)
	{
		selectedObject.ReCompileShader();
	}
}

void Inspector::ClickOpenTexture()
{
	if (!engineEventModel)
		return;

	QString filename = QFileDialog::getOpenFileName(this, tr("Select Texture File"), "",
		tr("BMP, DDS (*.bmp;*.dds);;All Texture Files (*.bmp;*.dds)"));

	Object& selectedObject = engineEventModel->GetSelectObject();
	if (filename.size() > 0 && &selectedObject)
	{
		int index = filename.indexOf("/Assets/");
		QString subString = filename.mid(index + 1);

		ui.TexPath->setText(subString);
		
		char pathBuffer[1024] = { 0, };
		qsnprintf(pathBuffer, sizeof(pathBuffer), "%s", subString.toUtf8().constData());

		selectedObject.SetTexture(&pathBuffer[0]);
	}
}

void Inspector::ClickOpenMesh()
{
	if (!engineEventModel)
		return;

	QString filename = QFileDialog::getOpenFileName(this, tr("Select Mesh File"), "",
		tr("Obj (*.obj);;All Shader Files (*.obj)"));
	
	if (filename.size() > 0)
	{
		int index = filename.indexOf("/Assets/");
		QString subString = filename.mid(index + 1);
		ui.MeshPath->setText(subString);
		engineEventModel->ChangeMesh(subString);
	}
}

void Inspector::ClickSetKey()
{
	emit SetKey();
}

void Inspector::WidgetTextChanged(const QString &text)
{
	if (!engineEventModel)
		return;

	engineEventModel->WidgetTextChange(text);
}

void Inspector::FontSizeChanged(double size)
{
	if (!engineEventModel)
		return;

	engineEventModel->FontSizeChange(size);
}


void Inspector::UpdateInspector()
{
	if (!engineEventModel)
		return;

	Object& object = engineEventModel->GetSelectObject();
	if (&object)
	{		
		ui.objectName->setText(object.Name.c_str());

		// update transform....
		glm::vec3 Pos = object.transform.GetPosition();
		glm::vec3 Rot = object.transform.GetRotate();
		glm::vec3 Scale = object.transform.GetScale();

		Node& parentNode = engineEventModel->GetNode().parent();
		Object& parentObject = parentNode.GetNodeObject();
		if (&parentObject)
		{
			glm::mat4 childWorldInv = object.transform.GetInverseMatrix();
			glm::mat4 childLocalInv = Core::Transform::MultMatrix(parentObject.transform.WorldMatrix, childWorldInv);
			glm::mat4 childLocalMatrix = glm::inverse(childLocalInv);
			Pos = Core::Transform::GetPositionByMatrix(childLocalMatrix);
			Scale = Core::Transform::GetScaleByMatrix(childLocalMatrix);
			Rot = Core::Transform::GetRotateByMatrix(childLocalMatrix);
		}

		ui.tx->setValue(Pos.x);
		ui.ty->setValue(Pos.y);
		ui.tz->setValue(Pos.z);

		ui.rx->setValue(Rot.x);
		ui.ry->setValue(Rot.y);
		ui.rz->setValue(Rot.z);

		ui.sx->setValue(Scale.x);
		ui.sy->setValue(Scale.y);
		ui.sz->setValue(Scale.z);



		Color color = object.color.GetColor();
		ui.cr->setValue(color.R);
		ui.cg->setValue(color.G);
		ui.cb->setValue(color.B);
		ui.ca->setValue(color.A);
		ui.specular->setValue(color.Specular);

		ui.VshaderPath->setText(object.GetVertexShader());
		ui.FshaderPath->setText(object.GetFragmentShader());

		const char* texPath = engineEventModel->GetObjectTexturePath();
		ui.TexPath->setText(texPath);

		if (object.Type == OBJECTTYPE::BUTTON ||
			object.Type == OBJECTTYPE::LABEL)
		{
			glWidget& widget = dynamic_cast<glWidget&>(object);
			QString text = widget.GetText().c_str();
			ui.text->setText(text);
			ui.fontsize->setValue(widget.GetFontSize());

			Color textColor = widget.GetTextColor();
			ui.fcr->setValue(textColor.R);
			ui.fcg->setValue(textColor.G);
			ui.fcb->setValue(textColor.B);
			ui.fca->setValue(textColor.A);
		}
		else if (object.Type == OBJECTTYPE::MESH)
		{
			glMesh& mesh = dynamic_cast<glMesh&>(object);
			QString text = mesh.GetMeshPath().c_str();
			ui.MeshPath->setText(text);
		}
	}
}

