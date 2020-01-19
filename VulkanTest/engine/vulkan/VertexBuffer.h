#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>


class Loader;
class IndexBuffer;
class CommandBuffer;
class CommandPool;
class Buffer;


class VertexBuffer
{

public:

	// Vertex buffer
	VkBuffer m_Buffer;
	VkDeviceMemory m_Memory;
	VkDevice m_device;

	VertexBuffer(VkDevice device, VkPhysicalDevice hPhysicalDevice, Loader loader, IndexBuffer* indexBuffer,
		VkQueue graphicsQueue, CommandBuffer commandBuffer, CommandPool* commandPool);
	~VertexBuffer();

};
