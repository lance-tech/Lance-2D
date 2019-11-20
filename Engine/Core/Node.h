#pragma once
#include <list>
#include <vector>
#include <string>

#include "../GLObject/Object.h"
#include "Collision.h"

class Node;

typedef void* (*ExecuteMethod1)(Node& node, const void* source);
typedef void (*ExecuteMethod2)(Node& node, void* source1, void* source2);

void DuplicateNodes(Node& node, void* source1, void* source2);

void* SelectedObject(Node& node, const void* source);
void* FindByName(Node& node, const void* source);
void* FindByAddr(Node& node, const void* source);

void DrawObject(Node& node, void* source1, void* source2);
void ObjectPressed(Node& node, void* source1, void* source2);
void UpdateObjectComponents(Node& node, void* source1, void* source2);

class Node
{
public:
	Node();
	~Node();

	/*
	BFS
	*/
	void ExecuteReturn_B(ExecuteMethod1 method, void*& returnValue, const void* argument1 = nullptr);
	void ExecuteOneWay_B(ExecuteMethod2 method, void* argument1 = nullptr, void* argument2 = nullptr);
	
	/*
	DFS
	*/
	void ExecuteOneWay_D(ExecuteMethod2 method, void* argument1 = nullptr, void* argument2 = nullptr);

	inline Node& child(int number) {
		Node* childNode = nullptr;
		if (ChildNodes.size() > number&& ChildNodes.size() > 0)
		{
			childNode = ChildNodes[number];
		}

		return *childNode;
	}
	inline Node& parent() { return *ParentNode; }
	inline int childCount() const { return (int)ChildNodes.size(); }
	int childNumber() const;
	inline std::string data() const {
		if (object)
		{
			return object->Name;
		}
		else
		{
			return "";
		}
	}
	bool InsertChild(Object& object);
	bool insertChildren(int position, int count, int columns);
	void insertChild(int pos, Node& node);
	void AddChild(Object& object);

	bool removeChild(int row);
	void RemoveSubNodes(Node& node);
	void DestroyNode(Node& node);
	bool RejectNode(int row);
	void RejectNode(Node& node);

	inline std::vector<Node*>& GetChildNodes() { return ChildNodes; }
	inline Node& GetChildNode(int index) const { return *ChildNodes[index]; }
	inline Object& GetChildObject(int index) const { return *ChildNodes[index]->object; }
	inline Object& GetNodeObject() const { return *object; }
	void SetNodeObject(Object& object);
	
	std::vector<Node*>::iterator FindChildNodeIter(Node* node);
private:
	bool IsSelected;
	Object* object;
	Node* ParentNode;
	std::vector<Node*> ChildNodes;
};



