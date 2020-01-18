#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>


class Loader;
class Buffer;
class CommandBuffer;
class CommandPool;


class IndexBuffer
{

public:

	VkBuffer indexBuffer;
	VkDeviceMemory indexBufferMemory;

	void createIndexBuffer(VkPhysicalDevice hPhysicalDevice, VkDevice device, Loader loader, Buffer buffer,
		VkQueue graphicsQueue, CommandBuffer commandBuffer, CommandPool commandPool);

	void copyBuffer(VkDevice device, VkQueue graphicsQueue, CommandBuffer commandBuffer, CommandPool commandPool,
		VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);

};
