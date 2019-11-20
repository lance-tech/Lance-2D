#include "Transform.h"

Core::Transform::Transform() :
	X(0.0f), Y(0.0f), Z(0.0f),
	RX(0.0f), RY(0.0f), RZ(0.0f),
	SX(1.0f), SY(1.0f), SZ(1.0f),
	WorldMatrix(1.0f)
{
}

Core::Transform::~Transform()
{
}

void Core::Transform::SetMatrix(glm::mat4 matrix)
{
	WorldMatrix = matrix;
}

glm::mat4 Core::Transform::MultMatrix(glm::mat4 mat_l, glm::mat4 mat_r)
{
	glm::mat4 outMat;

	outMat[0][0] = (mat_l[0][0] * mat_r[0][0]) + (mat_l[0][1] * mat_r[1][0]) + (mat_l[0][2] * mat_r[2][0]) + (mat_l[0][3] * mat_r[3][0]);
	outMat[0][1] = (mat_l[0][0] * mat_r[0][1]) + (mat_l[0][1] * mat_r[1][1]) + (mat_l[0][2] * mat_r[2][1]) + (mat_l[0][3] * mat_r[3][1]);
	outMat[0][2] = (mat_l[0][0] * mat_r[0][2]) + (mat_l[0][1] * mat_r[1][2]) + (mat_l[0][2] * mat_r[2][2]) + (mat_l[0][3] * mat_r[3][2]);
	outMat[0][3] = (mat_l[0][0] * mat_r[0][3]) + (mat_l[0][1] * mat_r[1][3]) + (mat_l[0][2] * mat_r[2][3]) + (mat_l[0][3] * mat_r[3][3]);

	outMat[1][0] = (mat_l[1][0] * mat_r[0][0]) + (mat_l[1][1] * mat_r[1][0]) + (mat_l[1][2] * mat_r[2][0]) + (mat_l[1][3] * mat_r[3][0]);
	outMat[1][1] = (mat_l[1][0] * mat_r[0][1]) + (mat_l[1][1] * mat_r[1][1]) + (mat_l[1][2] * mat_r[2][1]) + (mat_l[1][3] * mat_r[3][1]);
	outMat[1][2] = (mat_l[1][0] * mat_r[0][2]) + (mat_l[1][1] * mat_r[1][2]) + (mat_l[1][2] * mat_r[2][2]) + (mat_l[1][3] * mat_r[3][2]);
	outMat[1][3] = (mat_l[1][0] * mat_r[0][3]) + (mat_l[1][1] * mat_r[1][3]) + (mat_l[1][2] * mat_r[2][3]) + (mat_l[1][3] * mat_r[3][3]);

	outMat[2][0] = (mat_l[2][0] * mat_r[0][0]) + (mat_l[2][1] * mat_r[1][0]) + (mat_l[2][2] * mat_r[2][0]) + (mat_l[2][3] * mat_r[3][0]);
	outMat[2][1] = (mat_l[2][0] * mat_r[0][1]) + (mat_l[2][1] * mat_r[1][1]) + (mat_l[2][2] * mat_r[2][1]) + (mat_l[2][3] * mat_r[3][1]);
	outMat[2][2] = (mat_l[2][0] * mat_r[0][2]) + (mat_l[2][1] * mat_r[1][2]) + (mat_l[2][2] * mat_r[2][2]) + (mat_l[2][3] * mat_r[3][2]);
	outMat[2][3] = (mat_l[2][0] * mat_r[0][3]) + (mat_l[2][1] * mat_r[1][3]) + (mat_l[2][2] * mat_r[2][3]) + (mat_l[2][3] * mat_r[3][3]);

	outMat[3][0] = (mat_l[3][0] * mat_r[0][0]) + (mat_l[3][1] * mat_r[1][0]) + (mat_l[3][2] * mat_r[2][0]) + (mat_l[3][3] * mat_r[3][0]);
	outMat[3][1] = (mat_l[3][0] * mat_r[0][1]) + (mat_l[3][1] * mat_r[1][1]) + (mat_l[3][2] * mat_r[2][1]) + (mat_l[3][3] * mat_r[3][1]);
	outMat[3][2] = (mat_l[3][0] * mat_r[0][2]) + (mat_l[3][1] * mat_r[1][2]) + (mat_l[3][2] * mat_r[2][2]) + (mat_l[3][3] * mat_r[3][2]);
	outMat[3][3] = (mat_l[3][0] * mat_r[0][3]) + (mat_l[3][1] * mat_r[1][3]) + (mat_l[3][2] * mat_r[2][3]) + (mat_l[3][3] * mat_r[3][3]);

	return outMat;
}

//glm::mat4 Core::Transform::GetLocalMatrix()
//{
//	//WorldMatrix
//}


void Core::Transform::SetScreenPosition(float x, float y, float z)
{
	glm::vec3 pos = ConvertScreenToWorldPosition(x, y, z);
	SetWorldPosition(pos.x, pos.y, pos.z);
}

void Core::Transform::SetWorldPosition(float _x, float _y, float _z)
{
	SetMatrix(glm::mat4(1.0f));
	SetTranslate(_x, _y, _z);
}

void Core::Transform::SetTranslate(glm::vec3 pos)
{
	SetTranslate(pos.x, pos.y, pos.z);
}

void Core::Transform::SetTranslate(float x, float y, float z)
{
	X = x;
	Y = y;
	Z = z;

	glm::mat4 TranslationMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(X, Y, Z));
	glm::quat _quaternion(glm::vec3(glm::radians(RX), glm::radians(RY), glm::radians(RZ)));
	glm::mat4 rotationMatrix = glm::toMat4(_quaternion);
	glm::mat4 ScaleMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(SX, SY, SZ));
	glm::mat4 ModelMatrix = MultMatrix(MultMatrix(ScaleMatrix, rotationMatrix), TranslationMatrix);
	WorldMatrix = MultMatrix(ModelMatrix, WorldMatrix);
}

void Core::Transform::SetWorldRotate(float _x, float _y, float _z)
{
	SetMatrix(glm::mat4(1.0f));
	SetRotate(glm::vec3(_x, _y, _z));
}

void Core::Transform::SetRotate(glm::vec3 rotate)
{
	RX = rotate.x;
	RY = rotate.y;
	RZ = rotate.z;

	glm::mat4 TranslationMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(X, Y, Z));
	glm::quat _quaternion(glm::vec3(glm::radians(RX), glm::radians(RY), glm::radians(RZ)));
	glm::mat4 rotationMatrix = glm::toMat4(_quaternion);
	glm::mat4 ScaleMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(SX, SY, SZ));
	glm::mat4 ModelMatrix = MultMatrix(MultMatrix(ScaleMatrix, rotationMatrix), TranslationMatrix);
	WorldMatrix = MultMatrix(ModelMatrix, WorldMatrix);
}

void Core::Transform::SetRotate(float x, float y, float z)
{
	SetRotate(glm::vec3(x, y, z));
}

void Core::Transform::SetWorldScale(float x, float y, float z)
{
	SetMatrix(glm::mat4(1.0f));
	SetScale(x, y, z);
}

void Core::Transform::SetScale(float x, float y, float z)
{
	SX = x;
	SY = y;
	SZ = z;
	
	glm::mat4 TranslationMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(X, Y, Z));
	glm::quat _quaternion(glm::vec3(glm::radians(RX), glm::radians(RY), glm::radians(RZ)));
	glm::mat4 rotationMatrix = glm::toMat4(_quaternion);
	glm::mat4 ScaleMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(SX, SY, SZ));
	glm::mat4 ModelMatrix = MultMatrix(MultMatrix(ScaleMatrix, rotationMatrix), TranslationMatrix);
	WorldMatrix = MultMatrix(ModelMatrix, WorldMatrix);
}