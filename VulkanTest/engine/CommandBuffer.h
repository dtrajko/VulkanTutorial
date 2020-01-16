#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>


class CommandBuffer
{

public:

	VkCommandBuffer beginSingleTimeCommands(VkDevice device, VkCommandPool commandPool);
	void endSingleTimeCommands(VkDevice device, VkCommandBuffer cmdBuffer, VkQueue graphicsQueue, VkCommandPool commandPool);

};
