#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include "CommandPool.h"


class CommandBuffer
{

public:

	VkCommandBuffer beginSingleTimeCommands(VkDevice device, VkCommandPool commandPool);

	void endSingleTimeCommands(VkDevice device, VkCommandBuffer cmdBuffer, VkQueue graphicsQueue, VkCommandPool commandPool);

	void copyBufferToImage(VkDevice device, VkQueue graphicsQueue, CommandPool* commandPool, VkBuffer buffer, VkImage image, uint32_t width, uint32_t height);

};
