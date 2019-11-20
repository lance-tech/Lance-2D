#pragma once

#include "../Engine.h"

// GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/matrix_inverse.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/norm.hpp>


namespace Core
{
	class Transform
	{
	public:
		Transform();
		~Transform();

		Transform& operator=(const Transform &other)
		{
			this->X = other.X;
			this->Y = other.Y;
			this->Z = other.Z;
			this->RX = other.RX;
			this->RY = other.RY;
			this->RZ = other.RZ;
			this->SX = other.SX;
			this->SY = other.SY;
			this->SZ = other.SZ;
			this->WorldMatrix = other.WorldMatrix;
			return *this;
		}

		float X;
		float Y;
		float Z;

		float RX;
		float RY;
		float RZ;

		float SX;
		float SY;
		float SZ;

		glm::mat4 WorldMatrix;
		
		void SetMatrix(glm::mat4 matrix);
		static glm::mat4 MultMatrix(glm::mat4 mat_l, glm::mat4 mat_r);

		void SetTranslate(float x, float y, float z);
		void SetTranslate(glm::vec3 pos);
		void SetRotate(float x, float y, float z);
		void SetRotate(glm::vec3 rotate);
		void SetScale(float x, float y, float z = 1.0f);
		
		inline glm::vec3 GetPositionSnap() {
			float _x = floor(WorldMatrix[3][0] + 0.5f);
			float _y = floor(WorldMatrix[3][1] + 0.5f);
			float _z = floor(WorldMatrix[3][2] + 0.5f);
			return glm::vec3(_x, _y, _z);
		}
		inline glm::vec3 GetPosition() {
			float _x = WorldMatrix[3][0];
			float _y = WorldMatrix[3][1];
			float _z = WorldMatrix[3][2];
			return glm::vec3(_x, _y, _z);
		}
		inline glm::vec3 GetLocalPosition() { return glm::vec3(X, Y, Z); }
		inline glm::vec3 GetLocalPosition(glm::mat4 parentMatrix) {
			glm::mat4 InvMatrix = glm::inverse(WorldMatrix);
			glm::mat4 localInvMatrix = MultMatrix(parentMatrix, WorldMatrix);
			return GetPositionByMatrix(glm::inverse(localInvMatrix));
		}

		inline glm::vec3 GetRotate() {
			float theataX = glm::atan(WorldMatrix[1][2] / SY, WorldMatrix[2][2] / SZ);
			float degreeX = glm::degrees(theataX);

			float theataY = glm::atan((WorldMatrix[0][2] / SX) * -1,
				glm::sqrt(glm::pow(WorldMatrix[1][2] / SY, 2) + glm::pow(WorldMatrix[2][2] / SZ, 2)));
			float degreeY = glm::degrees(theataY);

			float theataZ = glm::atan(WorldMatrix[0][1] / SX, WorldMatrix[0][0] / SX);
			float degreeZ = glm::degrees(theataZ);

			return glm::vec3(degreeX, degreeY, degreeZ);
		}
		inline glm::vec3 GetLocalRotate() { return glm::vec3(RX, RY, RZ); }

		inline glm::vec3 GetScale() {
			float _x = glm::length(WorldMatrix[0]);
			float _y = glm::length(WorldMatrix[1]);
			float _z = glm::length(WorldMatrix[2]);

			return glm::vec3(_x, _y, _z);
		}
		inline glm::vec3 GetLocalScale() { return glm::vec3(SX, SY, SZ); }
		
		static inline glm::vec3 GetPositionByMatrix(glm::mat4 matrix) {
			float _x = matrix[3][0];
			float _y = matrix[3][1];
			float _z = matrix[3][2];
			return glm::vec3(_x, _y, _z);
		}

		static inline glm::vec3 GetScaleByMatrix(glm::mat4 matrix) {
			float _x = glm::length(matrix[0]);
			float _y = glm::length(matrix[1]);
			float _z = glm::length(matrix[2]);

			return glm::vec3(_x, _y, _z);
		}

		static inline glm::vec3 GetRotateByMatrix(glm::mat4 matrix) {
			glm::vec3 scale = GetScaleByMatrix(matrix);

			float theataX = glm::atan(matrix[1][2] / scale.y, matrix[2][2] / scale.z);
			float degreeX = glm::degrees(theataX);

			float theataY = glm::atan((matrix[0][2] / scale.x) * -1,
				glm::sqrt(glm::pow(matrix[1][2] / scale.y, 2) +
					glm::pow(matrix[2][2] / scale.z, 2)));
			float degreeY = glm::degrees(theataY);

			float theataZ = glm::atan(matrix[0][1] / scale.x, matrix[0][0] / scale.x);
			float degreeZ = glm::degrees(theataZ);

			return glm::vec3(degreeX, degreeY, degreeZ);
		}

		void SetWorldPosition(float _x, float _y, float _z = 0);
		void SetWorldRotate(float _x, float _y, float _z);
		void SetWorldScale(float x, float y, float z);

		inline glm::mat4 GetInverseMatrix() { return glm::inverse(WorldMatrix); }
		//glm::mat4 GetLocalMatrix();
		
		void SetScreenPosition(float x, float y, float z);
		inline glm::vec3 GetScreenPosition() {
			glm::vec3 worldPos = GetPosition();
			glm::vec3 screenPos;
			screenPos.x = worldPos.x + (WINDOW_LEFT * -1);
			screenPos.y = WINDOW_TOP - worldPos.y;
			screenPos.z = worldPos.z;

			return screenPos;
		}
		static inline glm::vec3 ConvertScreenToWorldPosition(float x, float y, float z) {
			return glm::vec3(x + WINDOW_LEFT, (WINDOW_TOP)-y, z);
		}
	};
}
