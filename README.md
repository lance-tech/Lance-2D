# OpenGL Game Engine (Lance2D)
**Simple game engine for 2D board game development using OpenGL.**

# Build on
- C++
- [Qt 5.13.1](https://download.qt.io/archive/qt/5.13/5.13.1/)
- [Qt Visual Studio Tools version 2.4.2](https://download.qt.io/archive/vsaddin/2.4.2/)
- [glew-2.1.0](http://glew.sourceforge.net/)
- [glfw-3.3](https://github.com/glfw/glfw/releases/download/3.3/glfw-3.3.bin.WIN64.zip)
- [glm](https://github.com/g-truc/glm/releases/tag/0.9.9.6)
- [freetype-2.10.0](https://www.freetype.org/download.html)

# Solution Structure
    Engine(Library) <---+ <---------------------+
                        | (Depends on build)    | (Link)
    Lance2D(Tool) ------+                       | 
                                                |
    Tetris(Game) -------------------------------+

# Introduction
It was developed for personal study purposes and is a free license.

Provides a simple map editor and enables real-time animations to be applied to game objects. Shaders can also be created in real time in the map tool by providing shader compilation.

Game object provides 2D plane and 3D object (OBJ) loader basically. In addition, the Tetris Block object was defined separately by specializing in the development of Tetris game.

If you want your own game object, you can create a new type of game object by inheriting it.

It also provides widgets for UI creation. Currently, only buttons and labels are provided, and buttons can be used by binding the callback method directly into the game scene code.

It provides user-defined components and can be added to a game object to configure independent logic at run-time.

It is easy to apply the hierarchical structure between game objects in the scene graph. By applying the basic 3D hierarchy, the child node inherits the transform properties of the parent node.

The scenes that made by yourself can be stored separately and each scene can be loaded by registering them in the scene manager at run-time.

  

# Review

## Game Life Cycle

## Map Tool

## Game Scene
For one game to work, you must inherit the game scene and define at least one new game scene. If you inherit the game scenes provided by the engine, you can use methods such as game logic, rendering, and input event.

The defined scene must be register to scene manager before the play for game.

After the register in scene manager, the instance is managed by the scene manager.

Below is a simple example.
```c++
#include <Scene/GameScene.h>

class MyGameScene : public GameScene
{
public:
    void InitializeGame() override;
	void GameLogic(double &time) override;
	void Render(glm::mat4 &VP, double &time) override;
};
```

```c++
MyGameScene *scene1 = new MyGameScene();
SceneManager::AddGameScene(*scene1);
```

## Event
By default, game scenes provide keyboard and mouse events.

These events can be used by overriding the functions below.

Below is a simple example.
```c++
void OnKeyPressed(int Event) override;
void OnKeyReleased(int Event) override;
void OnMousePressed(int Event, double x, double y) override;
void OnMouseMove(double x, double y) override;
void OnMouseReleased(int Event, double x, double y) override;
```

```c++
void MyGameScene::OnKeyPressed(int Event)
{
    switch (Event)
    {
    case KEY_UP:			
        /*
        Do Something...
        */
        break;
    default:
        break;
    }
}
```

## Animation
Animation is provide in two types. The first, the animation apply to the static object is played repeat within the entire frame.

This useful for sequence shot or repeat
animation.

Below is a simple example.

(...)

The second is an animation that lets you run on a specific event at run-time. This does not rely on the timeline of the scene, but has independent execution time. Therefore, it is possible to create an animation about the desired motion during run-time.

For example, if an animation is applied to a movement where a game object is deleted or moved, it may fade out and disappear or implement a linear movement.

Below is a simple example.

```c++
// Create the Anomator Instance that animation for some object
Animator* animator = new Animator();

// Assign to any keys what you want to mation
// This means to Start time of 0, at the 0 position
animator->animation.SetTranslateKey(0.0f, glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));   
// Move to 10 unit of y position during in 10 frames. 
animator->animation.SetTranslateKey(10.0f, glm::vec4(0.0f, 10.0f, 0.0f, 1.0f)); 

// Set target object that you want apply to animation.
animator->SetTargetObject(object);

// Add animator in animation manager
SceneManager::animationManager.AddAnimator(*animator);

// And finally you can play your animation in any other scope.
SceneManager::animationManager.PlayAnimators();
```

## Scene Graph
All objects (characters, enemies, obstacles, etc.) that want to appear in the game must be registered in the scene graph at the same time as they are created.

These registered objects are managed by a scene graph. Since both the memory and rendering of an object are managed by a scene graph, there is no need to additional code in the game scene.

You also do not need to invoke the constructor directly, but you can call it through the scene manager.

Below is a simple example.
```c++
TetrisBlock &tBlock = (TetrisBlock&)SceneManager::CreateObject(OBJECTTYPE::block0);
SceneRoot->RootNode->AddChild(tBlock);
```

```c++
Object &obejct = SceneRoot->FindNode("Object Name").GetNodeObject();
```

## Scene Graph – Advanced
Acyclic graph is a scene (DAG : Tree ) of nodes. There is only one top-level root node in the scene. One node contains a list of adjacent nodes and has a parent node pointer together. And it has a game object pointer, the actual data of the node.
```c++
class Node
{
private:
	Object* object;
	Node* ParentNode;
	std::vector<Node*> ChildNodes;
}
```
The game object doesn’t know which node it is located on. Therefore, if you want to For-loop the game objects, you have to start from the root node visit each child nodes.

The For-loop method supports DFS, BFS. To perform a specific action on a given element during a For-loop, work by handing over the function pointer you have defined.

Each For-loop method supports two types, one involving a return value and the other not.

Below is an example of the getting a selected object.
```c++
// Define the method according to the predefined function pointer of prototype
void* SelectedObject(Node& node, const void* source)
{
	Node* resNode = nullptr;
	if (node.GetNodeObject().IsSelected)
	{
		resNode = &node;
	}
	return resNode;
}

// How to use
// Define the return variable
void* returnValue = nullptr;
// Call function that execute method in root node,
// and send arguments the your function with variables.
RootNode->ExecuteReturn_B(&SelectedObject, returnValue);
// Casting the return variable to node
Node* node = (Node*)returnValue;
// If node pointer is not null, get object in the node instance.
Object* object = nullptr;
if (node)
{
    object = &node->GetNodeObject();
}
```

## Game Component
The game components is very useful if you want to control the behavior of a particular game object, such as adding new features to a game object or separating the logic of the game.

Game components can be used in add to all game objects.

Below is a simple example.
```c++
// Include GameComponent.h
#include <GameComponent/GameComponent.h>

// Defines the sub class of GameComponent
class ObjectBehaviour : public GameComponent
{
public:
	ObjectBehaviour();
	~ObjectBehaviour();
	virtual void Update(double &Time) override;

	Object* GameObject;
};
```

```c++
// Scene code
// Instanciate your GameComponent
ObjectBehaviour *objectBehaviour = new ObjectBehaviour();
// Set a specific name.
// Later, the component can be obtained through this name.
objectBehaviour->Name = "ObjectBehaviour";

// Set up a parent object to include that component.
// You don't need to set this variable if you don't need it for your logic.
objectBehaviour->GameObject = MyGameObject;

// Add components to the corresponding game object.
MyGameObject->AddComponent(objectBehaviour);
```

## UI
The UI button component used in the game must bind with callback function after creation.

Example of creating an object directly from a scene.
```c++
glButton &MyButton = (glButton&)SceneManager::CreateObject(BUTTON);
SceneRoot->RootNode->AddChild(MyButton);
MyButton.Connect(this, &MyGameScene::MyCallbackFunction);
```

```c++
glButton &MyButton = (glButton&)SceneRoot->FindNode("My Button 1").GetNodeObject();
MyButton.Connect(this, &MyGameScene::DoSomething);
```

## Shader
It is possible to compile a Vertex / Fragment Shader file in real time and check it in the viewport.


## Game Data
It’s a container that can manage the score of the game. It keeps the same information even after the game ends because it is saved as a file.

In addition, the highest/minimum score can be easily getting by using priority-queue internally.

Below is a simple example.
```c++
// The user's score and level are updated during the runtime game.
score++;
GameDataManager::SetUserScore(score);

level++;
GameDataManager::SetUserLevel(level);

// Save the current game data at the game over.
GameDataManager::SaveGameData();


// Data can be get from game data stored in other scenes, 
// ranking 1, 2 and 3.
1st->SetText(GameDataManager::GetTopScore());
2nd->SetText(GameDataManager::GetTopScore());
3rd->SetText(GameDataManager::GetTopScore());
```

# Tetris (Sample Game)

The Tetris game is a basic example game made using this engine.

As a major logic, it use object pooling techniques to efficiently manage the creation and deletion of blocks.

When cleaning the block rows, the animation was applied to create a dynamic movement.

Also, many of the graphic resources used in games are handled by shaders in real time without using textures, which can reduce the memory used for textures.

![CreatePlan](./screenshot/screenshot01.png)

![CreatePlan](./screenshot/screenshot01.png)

![CreatePlan](./screenshot/screenshot01.png)
