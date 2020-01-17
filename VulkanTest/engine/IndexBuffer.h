#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>


class CommandBuffer;
class CommandPool;

class IndexBuffer
{
public:

	void copyBuffer(VkDevice device, VkQueue graphicsQueue, CommandBuffer commandBuffer, CommandPool commandPool,
		VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);

};
