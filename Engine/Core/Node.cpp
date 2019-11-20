#include "Node.h"
#include "../Controller/SceneManager.h"
#include "Transform.h"


void DuplicateNodes(Node& node, void* source1, void* source2)
{
	Object& sourceObject = node.GetNodeObject();
	Object& newObject = SceneManager::CreateObject(sourceObject.Type);

	Object& parentObject = node.parent().GetNodeObject();
	Node* parentNode = &node.parent();
	if (&parentObject) 
	{
		Node& _parentNode = SceneManager::GetSceneRoot().FindNode(parentObject.Name + "_copyied");
		if (&_parentNode)
		{
			parentNode = &_parentNode;
		}
	}

	parentNode->InsertChild(newObject);

	newObject.transform = sourceObject.transform;
	newObject.animation = sourceObject.animation;
	newObject.color = sourceObject.color;
	newObject.Name = sourceObject.Name + "_copyied";
	newObject.SetVisible(sourceObject.GetVisible());
	newObject.SetShader(*sourceObject.GetShader());
	std::vector<GameComponent*> Components = sourceObject.GetComponents();
	for (const auto& component : Components)
	{
		newObject.AddComponent(component);
	}

	if (sourceObject.Type == OBJECTTYPE::BUTTON ||
		sourceObject.Type == OBJECTTYPE::LABEL)
	{
		glWidget& originWidget = dynamic_cast<glWidget&>(sourceObject);
		glWidget& copiedWidget = dynamic_cast<glWidget&>(newObject);
		copiedWidget.SetText(originWidget.GetText());
		copiedWidget.SetFontSize(originWidget.GetFontSize());
	}
	else if (sourceObject.Type == OBJECTTYPE::MESH)
	{
		glMesh& originMesh = dynamic_cast<glMesh&>(sourceObject);
		glMesh& copiedMesh = dynamic_cast<glMesh&>(newObject);

		copiedMesh.InitializeMesh(originMesh.GetMeshPath());
	}
}

void* SelectedObject(Node& node, const void* source)
{
	Node* resNode = nullptr;
	if (node.GetNodeObject().IsSelected)
	{
		resNode = &node;
	}
	return resNode;
}


void* FindByName(Node& node, const void* source)
{
	Node* resNode = nullptr;
	const std::string &sourceName = *(static_cast<const std::string*>(source));
	if (node.GetNodeObject().Name == sourceName)
	{
		resNode = &node;
	}
	return resNode;
}

void* FindByAddr(Node& node, const void* source)
{
	Node* resNode = nullptr;
	if (&node.GetNodeObject() == source)
	{
		resNode = &node;
	}
	return resNode;
}

void DrawObject(Node& node, void* source1, void* source2)
{
	Object& object = node.GetNodeObject();
	glm::mat4 VP = *(static_cast<glm::mat4*>(source1));
	double* time = static_cast<double*>(source2);
	if (object.GetVisible())
	{
		object.Draw(VP, *time);
	}
}

void ObjectPressed(Node& node, void* source1, void* source2)
{
	Object& object = node.GetNodeObject();
	double* x = static_cast<double*>(source1);
	double* y = static_cast<double*>(source2);
	if (Collision::CollisionWithPoint(object, *x, *y))
	{
		object.SetSelected(true);
		object.pressed();
	}
	else
	{
		object.SetSelected(false);
	}
}

void UpdateObjectComponents(Node& node, void* source1, void* source2)
{
	Object& object = node.GetNodeObject();
	double* time = static_cast<double*>(source1);
	object.UpdateComponents(*time);
}


Node::Node() : 
	IsSelected(false),
	object(nullptr),
	ParentNode(nullptr)
{
}

Node::~Node()
{
}

int Node::childNumber() const
{
	if (ParentNode)
	{
		std::vector<Node*>::iterator it =
			std::find(ParentNode->ChildNodes.begin(), ParentNode->ChildNodes.end(), this);

		if (it != ParentNode->ChildNodes.end())
		{
			int index = (int)std::distance(ParentNode->ChildNodes.begin(), it);
			return index;
		}
	}
	return 0;
}

bool Node::insertChildren(int position, int count, int columns)
{
	if (position < 0 || position > ChildNodes.size())
		return false;

	for (int row = 0; row < count; ++row) {
		Node *node = new Node();
		node->ParentNode = this;
		ChildNodes.push_back(node);
	}

	return true;
}

bool Node::InsertChild(Object& object)
{
	Node *node = new Node();
	node->ParentNode = this;
	node->SetNodeObject(object);
	ChildNodes.push_back(node);

	return true;
}

void Node::insertChild(int pos, Node& node)
{
	ChildNodes.insert(ChildNodes.begin() + pos, &node);
	node.ParentNode = this;
}

void Node::AddChild(Object& object)
{
	Node *node = new Node();
	node->ParentNode = this;
	node->object = &object;
	ChildNodes.push_back(node);
}

bool Node::RejectNode(int row)
{
	if (ChildNodes.size() > row && ChildNodes.size() > 0)
	{
		ChildNodes.erase(ChildNodes.begin() + row);
		return true;
	}
	return false;
}

bool Node::removeChild(int row)
{
	if (ChildNodes.size() > row && ChildNodes.size() > 0)
	{
		Node& childNode = child(row);
		RemoveSubNodes(childNode);
		return true;
	}
	return false;
}

void Node::RemoveSubNodes(Node& node)
{
	int childCount = node.childCount();
	for (int i = 0; i < childCount; i++)
	{
		RemoveSubNodes(*node.ChildNodes[0]);
	}

	if (node.object)
	{
		DestroyNode(node);
	}
}

void Node::DestroyNode(Node& node)
{
	delete node.object;
	node.object = NULL;

	RejectNode(node);
}

void Node::RejectNode(Node& node)
{
	std::vector<Node*>::iterator curNodeIter = std::find(
		node.ParentNode->ChildNodes.begin(),
		node.ParentNode->ChildNodes.end(),
		&node);

	node.ParentNode->ChildNodes.erase(curNodeIter);
}

void Node::SetNodeObject(Object& object)
{
	this->object = &object;
}

std::vector<Node*>::iterator Node::FindChildNodeIter(Node* node)
{
	std::vector<Node*>::iterator childNodeIter = std::find(
		ParentNode->ChildNodes.begin(),
		ParentNode->ChildNodes.end(),
		node);

	return childNodeIter;
}

void Node::ExecuteReturn_B(ExecuteMethod1 method, void*& returnValue, const void* argument1)
{
	std::list<Node*> queue;
	queue.push_back(this);

	std::vector<Node*>::iterator it;
	while (!queue.empty())
	{
		Node* currentNode = queue.front();

		if (currentNode->object)
		{
			void* res = method(*currentNode, argument1);
			returnValue = res;
			if (returnValue)
			{
				break;
			}
		}

		queue.pop_front();

		for (it = currentNode->ChildNodes.begin(); it != currentNode->ChildNodes.end(); it++)
		{
			Node* adjNode = *it;
			queue.push_back(adjNode);
		}
	}
}

void Node::ExecuteOneWay_B(ExecuteMethod2 method, void* argument1, void* argument2)
{
	std::list<Node*> queue;
	queue.push_back(this);

	std::vector<Node*>::iterator it;
	while (!queue.empty())
	{
		Node* currentNode = queue.front();

		if (currentNode->object)
		{
			method(*currentNode, argument1, argument2);
		}
		queue.pop_front();
		for (it = currentNode->ChildNodes.begin(); it != currentNode->ChildNodes.end(); it++)
		{
			Node* adjNode = *it;
			queue.push_back(adjNode);
		}
	}
}

void Node::ExecuteOneWay_D(ExecuteMethod2 method, void* argument1, void* argument2)
{
	std::list<Node*> stack;
	stack.push_back(this);

	std::vector<Node*>::iterator it;
	while (!stack.empty())
	{
		Node* currentNode = stack.back();

		if (currentNode->object)
		{
			method(*currentNode, argument1, argument2);
		}
		stack.pop_back();
		for (it = currentNode->ChildNodes.begin(); it != currentNode->ChildNodes.end(); it++)
		{
			Node* adjNode = *it;
			stack.push_back(adjNode);
		}
	}
}