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

	VkBuffer m_Buffer;
	VkDeviceMemory m_Memory;
	VkDevice m_Device;

	IndexBuffer(VkDevice device, VkPhysicalDevice hPhysicalDevice, Loader* loader, Buffer* buffer,
		VkQueue graphicsQueue, CommandBuffer commandBuffer, CommandPool* commandPool);

	~IndexBuffer();

	void copyBuffer(VkDevice device, VkQueue graphicsQueue, CommandBuffer commandBuffer, CommandPool* commandPool,
		VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);

};
