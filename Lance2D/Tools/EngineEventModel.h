#pragma once
#include <QObject>

#include <vector>

#include <Core/Node.h>
#include <Core/TimeLineModel.h>


class EngineEventModel : public QObject
{
	Q_OBJECT

signals:
	void ObjectNameChanged(const QString &name);
	void UpdateTimeInfo();

public:
	EngineEventModel(QObject *parent = Q_NULLPTR);
	~EngineEventModel() {}

	std::vector<int> Frames;
	float InputPosX = 0.f;
	float InputPosY = 0.f;

	TimeLineModel* Timeline;

	inline Object& GetSelectObject()
	{
		return *SelectedObject;
	}
	inline void SetObject(Object* object)
	{
		SelectedObject = object;
	}

	inline Node& GetNode()
	{
		return *SelectedNode;
	}
	inline void SetNode(Node* node)
	{
		SelectedNode = node;
	}
	inline bool HasNode()
	{
		if (SelectedNode)
		{
			return true;
		}
		return false;
	}
	inline bool HasObject()
	{
		if (SelectedObject)
		{
			return true;
		}
		return false;
	}

	void SetObjectName(const QString& name);
	void SetShader(QString& name);
	void ValueChanged(QString propertyName, double value);
	void SetKey(QString propertyName, double value);
	void ChangeMesh(QString& name);
	void WidgetTextChange(const QString &text);
	void FontSizeChange(double value);
	const char* GetObjectTexturePath();
	void ChangedObjectTransform();
	void SetTimeModel(TimeLineModel& timeModel);

private:
	Object* SelectedObject = nullptr;
	Node* SelectedNode = nullptr;

	void SetObjectPosition(float x, float y, float z);
	void SetObjectScale(float x, float y, float z);
	void SetObjectRotate(float x, float y, float z);

	int snap;
	glm::vec3 ConvertSnapValue(float x, float y, float z=0.0f);

public slots:
	void SetSnapTranslate(int state);
};

