#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>


class CommandBuffer
{

public:

	// Command pool
	VkCommandPool commandPool;

	VkCommandBuffer beginSingleTimeCommands(VkDevice device);

};
