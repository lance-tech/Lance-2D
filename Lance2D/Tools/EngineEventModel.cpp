#pragma once
#include "EngineEventModel.h"
#include <GLObject/glWidget.h>
#include <Controller/SceneManager.h>
#include <Controller/MaterialManager.h>


EngineEventModel::EngineEventModel(QObject *parent)
	: QObject(parent), Timeline(nullptr), snap(0)
{

}

void EngineEventModel::SetObjectName(const QString& name)
{
	if (HasObject())
	{
		SelectedObject->Name = name.toUtf8().constData();

		emit ObjectNameChanged(name);
	}
}

void EngineEventModel::SetShader(QString& name)
{
	if (HasObject())
	{
		bool isVertexShader = name.contains(".vert", Qt::CaseInsensitive);
		if (isVertexShader)
		{
			MaterialManager::ReCompileShader(SelectedObject->GetShader()->GetID(),
				name.toUtf8().constData(),
				SelectedObject->GetFragmentShader());
		}
		else
		{
			MaterialManager::ReCompileShader(SelectedObject->GetShader()->GetID(),
				SelectedObject->GetVertexShader(),
				name.toUtf8().constData());
		}
	}
}

void EngineEventModel::ChangeMesh(QString& name)
{
	if (HasObject())
	{
		glMesh& mesh = dynamic_cast<glMesh&>(*SelectedObject);
		mesh.InitializeMesh(name.toUtf8().constData());
	}
}

void EngineEventModel::WidgetTextChange(const QString &text)
{
	if (HasObject())
	{
		if (SelectedObject->Type == OBJECTTYPE::BUTTON ||
			SelectedObject->Type == OBJECTTYPE::LABEL)
		{
			glWidget& widget = dynamic_cast<glWidget&>(*SelectedObject);
			widget.SetText(text.toUtf8().constData());
		}
	}
}

void EngineEventModel::FontSizeChange(double value)
{
	if (HasObject())
	{
		if (SelectedObject->Type == OBJECTTYPE::BUTTON ||
			SelectedObject->Type == OBJECTTYPE::LABEL)
		{
			glWidget& widget = dynamic_cast<glWidget&>(*SelectedObject);
			widget.SetFontSize(value);
		}
	}
}

const char* EngineEventModel::GetObjectTexturePath()
{
	const char* texturePath = SelectedObject->GetShader()->GetTexturePath();
	return texturePath;
}


void EngineEventModel::ValueChanged(QString propertyName, double value)
{
	//qDebug() << SelectedObject->Name.c_str() << " : " << propertyName << " : " << value;

	if (propertyName == "cr")
	{
		SelectedObject->color.R = value;
	}
	else if (propertyName == "cg")
	{
		SelectedObject->color.G = value;
	}
	else if (propertyName == "cb")
	{
		SelectedObject->color.B = value;
	}
	else if (propertyName == "ca")
	{
		SelectedObject->color.A = value;
	}

	else if (propertyName == "specular")
	{
		SelectedObject->color.Specular = value;
	}

	else if (propertyName == "fcr")
	{
		if (SelectedObject->Type == OBJECTTYPE::LABEL ||
			SelectedObject->Type == OBJECTTYPE::BUTTON)
		{
			glWidget& widget = dynamic_cast<glWidget&>(*SelectedObject);
			glText& text = widget.GetTextObject();
			text.color.R = value;
		}
	}
	else if (propertyName == "fcg")
	{
		if (SelectedObject->Type == OBJECTTYPE::LABEL ||
			SelectedObject->Type == OBJECTTYPE::BUTTON)
		{
			glWidget& widget = dynamic_cast<glWidget&>(*SelectedObject);
			glText& text = widget.GetTextObject();
			text.color.G = value;
		}
	}
	else if (propertyName == "fcb")
	{
		if (SelectedObject->Type == OBJECTTYPE::LABEL ||
			SelectedObject->Type == OBJECTTYPE::BUTTON)
		{
			glWidget& widget = dynamic_cast<glWidget&>(*SelectedObject);
			glText& text = widget.GetTextObject();
			text.color.B = value;
		}
	}
	else if (propertyName == "fca")
	{
		if (SelectedObject->Type == OBJECTTYPE::LABEL ||
			SelectedObject->Type == OBJECTTYPE::BUTTON)
		{
			glWidget& widget = dynamic_cast<glWidget&>(*SelectedObject);
			glText& text = widget.GetTextObject();
			text.color.A = value;
		}
	}

	else if (propertyName == "tx")
	{
		glm::vec3 pos = SelectedObject->transform.GetLocalPosition();
		SetObjectPosition(value, pos.y, pos.z);
	}
	else if (propertyName == "ty")
	{
		glm::vec3 pos = SelectedObject->transform.GetLocalPosition();
		SetObjectPosition(pos.x, value, pos.z);
	}
	else if (propertyName == "tz")
	{
		glm::vec3 pos = SelectedObject->transform.GetLocalPosition();
		SetObjectPosition(pos.x, pos.y, value);
	}

	else if (propertyName == "sx")
	{
		glm::vec3 scale = SelectedObject->transform.GetLocalScale();
		SetObjectScale(value, scale.y, scale.z);
	}
	else if (propertyName == "sy")
	{
		glm::vec3 scale = SelectedObject->transform.GetLocalScale();
		SetObjectScale(scale.x, value, scale.z);
	}
	else if (propertyName == "sz")
	{
		glm::vec3 scale = SelectedObject->transform.GetLocalScale();
		SetObjectScale(scale.x, scale.y, value);
	}

	else if (propertyName == "rx")
	{
		glm::vec3 rot = SelectedObject->transform.GetLocalRotate();
		SetObjectRotate(value, rot.y, rot.z);
	}
	else if (propertyName == "ry")
	{
		glm::vec3 rot = SelectedObject->transform.GetLocalRotate();
		SetObjectRotate(rot.x, value, rot.z);
	}
	else if (propertyName == "rz")
	{
		glm::vec3 rot = SelectedObject->transform.GetLocalRotate();
		SetObjectRotate(rot.x, rot.y, value);
	}
}

void EngineEventModel::SetObjectRotate(float x, float y, float z)
{
	glm::vec3 pos = ConvertSnapValue(x, y, z);

	Node& node = SceneManager::GetSceneRoot().FindNode(SelectedObject->Name);
	Node& parentNode = node.parent();
	Object& parentObject = parentNode.GetNodeObject();
	if (&parentObject)
	{
		SelectedObject->transform.SetMatrix(parentObject.transform.WorldMatrix);
		SelectedObject->transform.SetRotate(pos.x, pos.y, pos.z);
	}
	else
	{
		SelectedObject->transform.SetWorldRotate(pos.x, pos.y, pos.z);
	}
}

void EngineEventModel::SetObjectPosition(float x, float y, float z)
{
	glm::vec3 pos = ConvertSnapValue(x, y, z);

	Node& node = SceneManager::GetSceneRoot().FindNode(SelectedObject->Name);
	Node& parentNode = node.parent();
	Object& parentObject = parentNode.GetNodeObject();
	if (&parentObject)
	{
		SelectedObject->transform.SetMatrix(parentObject.transform.WorldMatrix);
		SelectedObject->transform.SetTranslate(pos.x, pos.y, pos.z);
	}
	else
	{
		SelectedObject->transform.SetWorldPosition(pos.x, pos.y, pos.z);
	}
}

void EngineEventModel::SetObjectScale(float x, float y, float z)
{
	glm::vec3 pos = ConvertSnapValue(x, y, z);

	Node& node = SceneManager::GetSceneRoot().FindNode(SelectedObject->Name);
	Node& parentNode = node.parent();
	Object& parentObject = parentNode.GetNodeObject();
	if (&parentObject)
	{
		SelectedObject->transform.SetMatrix(parentObject.transform.WorldMatrix);
		SelectedObject->transform.SetScale(pos.x, pos.y, pos.z);
	}
	else
	{
		SelectedObject->transform.SetWorldScale(pos.x, pos.y, pos.z);
	}
}

void EngineEventModel::SetKey(QString propertyName, double value)
{
	glm::vec4 key;
	glm::vec3 pos = SelectedObject->transform.GetLocalPosition();
	glm::vec3 rot = SelectedObject->transform.GetLocalRotate();
	glm::vec3 scale = SelectedObject->transform.GetLocalScale();
	Color color = SelectedObject->color.GetColor();

	if (propertyName == "tx")
	{
		key.x = value;
		key.y = pos.y;
		key.z = pos.z;
		SelectedObject->animation.SetTranslateKey(Timeline->CurrentFrame, key);
	}
	else if (propertyName == "ty")
	{
		key.x = pos.x;
		key.y = value;
		key.z = pos.z;
		SelectedObject->animation.SetTranslateKey(Timeline->CurrentFrame, key);
	}

	else if (propertyName == "rx")
	{
		key.x = value;
		key.y = rot.y;
		key.z = rot.z;
		SelectedObject->animation.SetRotateKey(Timeline->CurrentFrame, key);
	}
	else if (propertyName == "ry")
	{
		key.x = rot.x;
		key.y = value;
		key.z = rot.z;
		SelectedObject->animation.SetRotateKey(Timeline->CurrentFrame, key);
	}
	else if (propertyName == "rz")
	{
		key.x = rot.x;
		key.y = rot.y;
		key.z = value;
		SelectedObject->animation.SetRotateKey(Timeline->CurrentFrame, key);
	}

	else if (propertyName == "sx")
	{
		key.x = value;
		key.y = scale.y;
		key.z = scale.z;
		SelectedObject->animation.SetScaleKey(Timeline->CurrentFrame, key);
	}
	else if (propertyName == "sy")
	{
		key.x = scale.x;
		key.y = value;
		key.z = scale.z;
		SelectedObject->animation.SetScaleKey(Timeline->CurrentFrame, key);
	}
	else if (propertyName == "sz")
	{
		key.x = scale.x;
		key.y = scale.y;
		key.z = value;
		SelectedObject->animation.SetScaleKey(Timeline->CurrentFrame, key);
	}

	else if (propertyName == "cr")
	{
		key.r = value;
		key.g = color.G;
		key.b = color.B;
		key.a = color.A;
		SelectedObject->animation.SetColorKey(Timeline->CurrentFrame, key);
	}
	else if (propertyName == "cg")
	{
		key.r = color.R;
		key.g = value;
		key.b = color.B;
		key.a = color.A;
		SelectedObject->animation.SetColorKey(Timeline->CurrentFrame, key);
	}
	else if (propertyName == "cb")
	{
		key.r = color.R;
		key.g = color.G;
		key.b = value;
		key.a = color.A;
		SelectedObject->animation.SetColorKey(Timeline->CurrentFrame, key);
	}
	else if (propertyName == "ca")
	{
		key.r = color.R;
		key.g = color.G;
		key.b = color.B;
		key.a = value;
		SelectedObject->animation.SetColorKey(Timeline->CurrentFrame, key);
	}
}

void EngineEventModel::ChangedObjectTransform()
{
	glm::vec3 pos = ConvertSnapValue(InputPosX, InputPosY, SelectedObject->transform.GetPosition().z);

	glm::vec3 inputWorldPos = Core::Transform::ConvertScreenToWorldPosition(pos.x, pos.y, pos.z);
	Node& parentNode = GetNode().parent();
	Object& parentObject = parentNode.GetNodeObject();
	if (&parentObject)
	{
		glm::mat4 inputWorldMatrix(1.0f);
		inputWorldMatrix[3][0] = inputWorldPos.x;
		inputWorldMatrix[3][1] = inputWorldPos.y;
		inputWorldMatrix[3][2] = inputWorldPos.z;

		SelectedObject->transform.SetMatrix(parentObject.transform.WorldMatrix);

		glm::mat4 inputLocalInv = Core::Transform::MultMatrix(parentObject.transform.WorldMatrix, glm::inverse(inputWorldMatrix));
		glm::mat4 inputLocalMatrix = glm::inverse(inputLocalInv);
		glm::vec3 localPos = Core::Transform::GetPositionByMatrix(inputLocalMatrix);

		SelectedObject->transform.SetTranslate(localPos);
	}
	else
	{
		SelectedObject->transform.SetWorldPosition(inputWorldPos.x, inputWorldPos.y, inputWorldPos.z);
	}
}

void EngineEventModel::SetTimeModel(TimeLineModel& timeModel)
{
	Timeline = &timeModel;

	emit UpdateTimeInfo();
}

void EngineEventModel::SetSnapTranslate(int state)
{
	snap = state;
}

glm::vec3 EngineEventModel::ConvertSnapValue(float x, float y, float z)
{
	glm::vec3 outValue(x, y, z);
	if (snap == Qt::Checked)
	{
		float _x = floor(x + 0.5f);
		float _y = floor(y + 0.5f);
		float _z = floor(z + 0.5f);
		outValue.x = _x;
		outValue.y = _y;
		outValue.z = _z;
	}
	return outValue;
}