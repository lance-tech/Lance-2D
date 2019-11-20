#pragma once
#include "../GLObject/Object.h"
#include "../Core/Node.h"
#include <list>
#include <vector>

class SceneGraph
{
public:
	SceneGraph();
	~SceneGraph();

	Object& GetSelectedObject();
	Node& FindNode(const Object& object);
	Node& FindNode(const std::string& name);
	void DeleteNode(Node& node);

	Node *RootNode;

	void InheritTransform(const Node &node);


};