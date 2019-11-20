#include "BlockBehaviour.h"



BlockBehaviour::BlockBehaviour() : Block(nullptr)
{
}


BlockBehaviour::~BlockBehaviour()
{
}


void BlockBehaviour::Update(double &Time)
{
}

void BlockBehaviour::MoveVertical(float y)
{
	glm::vec3 pos = Block->transform.GetPositionSnap();
	pos.y = pos.y + y;
	Block->transform.SetWorldPosition(pos.x, pos.y, pos.z);
	Block->initTetrisBlockShape();
}

void BlockBehaviour::MoveHorizontal(int direction)
{
	glm::vec3 pos = Block->transform.GetPositionSnap();
	pos.x = pos.x + direction;
	Block->transform.SetWorldPosition(pos.x, pos.y, pos.z);
	Block->initTetrisBlockShape();
}

void BlockBehaviour::Rotate(float degree)
{
	glm::vec3 rot = Block->transform.GetLocalRotate();
	rot.z = rot.z + degree;
	Block->transform.SetWorldRotate(rot.x, rot.y, rot.z);
	Block->initTetrisBlockShape();
}