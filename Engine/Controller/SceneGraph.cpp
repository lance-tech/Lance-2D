#include "SceneGraph.h"

Node& SceneGraph::FindNode(const std::string& name)
{
	void* returnValue = nullptr;
	RootNode->ExecuteReturn_B(&FindByName, returnValue, static_cast<const void*>(&name));
	Node* node = nullptr;
	if (returnValue)
	{
		node = (Node*)returnValue;
	}
	return *node;
}

Node& SceneGraph::FindNode(const Object& object)
{
	void* returnValue = nullptr;
	RootNode->ExecuteReturn_B(&FindByAddr, returnValue, &object);
	Node* node = nullptr;
	if (returnValue)
	{
		node = (Node*)returnValue;
	}
	return *node;
}

Object& SceneGraph::GetSelectedObject()
{
	void* returnValue = nullptr;
	RootNode->ExecuteReturn_B(&SelectedObject, returnValue);
	Node* node = (Node*)returnValue;
	Object* object = nullptr;
	if (node)
	{
		object = &node->GetNodeObject();
	}
	return *object;
}

SceneGraph::SceneGraph()
{
	RootNode = new Node();
}

SceneGraph::~SceneGraph()
{
	int childCount = RootNode->childCount();
	for (int i = 0; i < childCount; i++)
	{
		RootNode->RemoveSubNodes(RootNode->GetChildNode(0));
	}
	delete RootNode;
}

void SceneGraph::DeleteNode(Node& node)
{
	int childs = node.childCount();
	for (int i = 0; i < childs; i++)
	{
		DeleteNode(node.GetChildNode(0));
	}

	Object& object = node.GetNodeObject();
	if (&object)
	{
		node.DestroyNode(node);
	}
}

void SceneGraph::InheritTransform(const Node &node)
{
	for (int i = 0; i < node.childCount(); i++)
	{
		Object& childObject = node.GetChildObject(i);
		Object& object = node.GetNodeObject();

		childObject.transform.SetMatrix(object.transform.WorldMatrix);

		glm::vec3 localPos = childObject.transform.GetLocalPosition();
		glm::vec3 localScale = childObject.transform.GetLocalScale();
		glm::vec3 localRotate = childObject.transform.GetLocalRotate();

		
		glm::mat4 TranslationMatrix = glm::translate(glm::mat4(1.0f), localPos);
		glm::quat _quaternion(glm::vec3(glm::radians(localRotate.x), glm::radians(localRotate.y), glm::radians(localRotate.z)));
		glm::mat4 rotationMatrix = glm::toMat4(_quaternion);
		glm::mat4 ScaleMatrix = glm::scale(glm::mat4(1.0f), localScale);

		glm::mat4 ModelMatrix = Core::Transform::MultMatrix(
			Core::Transform::MultMatrix(ScaleMatrix, rotationMatrix), TranslationMatrix);

		childObject.transform.WorldMatrix = Core::Transform::MultMatrix(ModelMatrix, childObject.transform.WorldMatrix);


		childObject.selectColor = object.selectColor;
		InheritTransform(node.GetChildNode(i));
	}
}
