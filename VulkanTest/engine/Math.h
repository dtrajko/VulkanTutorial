#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>


class Math
{
public:

	static glm::mat4 createViewMatrix(float camPitch, float camYaw, float camRoll, float posX, float posY, float posZ)
	{
		glm::mat4 viewMatrix;
		viewMatrix = glm::rotate(viewMatrix, glm::radians(camPitch), glm::vec3(1, 0, 0));
		viewMatrix = glm::rotate(viewMatrix, glm::radians(camYaw), glm::vec3(0, 1, 0));
		viewMatrix = glm::rotate(viewMatrix, glm::radians(camRoll), glm::vec3(0, 0, 1));
		glm::vec3 cameraPos = glm::vec3(posX, posY, posZ);
		glm::vec3 negativeCameraPos = -cameraPos;
		viewMatrix = glm::translate(viewMatrix, negativeCameraPos);
		return viewMatrix;
	}
};
