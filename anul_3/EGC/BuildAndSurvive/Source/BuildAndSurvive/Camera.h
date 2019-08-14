#pragma once
#include <include/glm.h>
#include <include/math.h>


class Camera
{
	public:
		Camera()
		{
			position = glm::vec3(0, 0, 0);
			forward = glm::vec3(0, 0, -1);
			up		= glm::vec3(0, 1, 0);
			right	= glm::vec3(1, 0, 0);
			distanceToTarget = 3;
		}

		Camera(const glm::vec3 &position, const glm::vec3 &center, const glm::vec3 &up)
		{
			Set(position, center, up);
		}

		~Camera()
		{ }

		// Update camera
		void Set(const glm::vec3 &position, const glm::vec3 &center, const glm::vec3 &up)
		{
			this->position = position;
			forward = glm::normalize(center - position);
			right	= glm::cross(forward, up);
			this->up = glm::cross(right, forward);
		}

		void MoveForward(float distance)
		{
			glm::vec3 dir = glm::normalize(glm::vec3(forward.x, 0, forward.z));
            this->position += dir * distance;
		}

		void TranslateForward(float distance)
		{
            this->position += glm::normalize(forward) * distance;
		}

		void TranslateToTarget(float distance)
		{
            this->position += glm::normalize(forward) * distanceToTarget * distance;
		}


		void TranslateUpword(float distance)
		{
            this->position += glm::normalize(up) * distance;
		}

		void TranslateRight(float distance)
		{
            this->position += glm::normalize(right) * distance;
		}

		void RotateFirstPerson_OX(float angle)
		{
            forward = glm::normalize(glm::vec3(glm::rotate(glm::mat4(1.0f), angle, right) * glm::vec4(forward, 1)));
            up = glm::normalize(glm::vec3(glm::rotate(glm::mat4(1.0f), angle, right) * glm::vec4(up, 1)));
		}

		void RotateFirstPerson_OY(float angle)
		{
            forward = glm::normalize(glm::vec3(glm::rotate(glm::mat4(1.0f), angle, glm::vec3(0, 1, 0)) * glm::vec4(forward, 1)));
            up = glm::normalize(glm::vec3(glm::rotate(glm::mat4(1.0f), angle, glm::vec3(0, 1, 0)) * glm::vec4(up, 1)));
            right = glm::normalize(glm::vec3(glm::rotate(glm::mat4(1.0f), angle, glm::vec3(0, 1, 0)) * glm::vec4(right, 1)));
		}

		void RotateFirstPerson_OZ(float angle)
		{
            right = glm::normalize(glm::vec3(glm::rotate(glm::mat4(1.0f), angle, forward) * glm::vec4(right, 1)));
            up = glm::normalize(glm::vec3(glm::rotate(glm::mat4(1.0f), angle, forward) * glm::vec4(up, 1)));
		}

		void RotateThirdPerson_OX(float angle)
		{

            TranslateToTarget(1);
            RotateFirstPerson_OX(angle);
            TranslateToTarget(-1);

		}

		void RotateThirdPerson_OY(float angle)
		{
            TranslateToTarget(1);
            RotateFirstPerson_OY(angle);
            TranslateToTarget(-1);
        }

		void RotateThirdPerson_OZ(float angle)
		{
            TranslateToTarget(1);
            RotateFirstPerson_OZ(angle);
            TranslateToTarget(-1);
		}

		glm::mat4 GetViewMatrix()
		{
			return glm::lookAt(position, position + forward, up);
		}

		glm::vec3 GetTargetPosition()
		{
			return position + forward * distanceToTarget;
		}

	public:
		float distanceToTarget;
		glm::vec3 position;
		glm::vec3 forward;
		glm::vec3 right;
		glm::vec3 up;
};
