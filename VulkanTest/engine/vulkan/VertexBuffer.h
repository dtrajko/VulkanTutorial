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
	VkBuffer vertexBuffer;
	VkDeviceMemory vertexBufferMemory;

	void createVertexBuffer(VkPhysicalDevice hPhysicalDevice, VkDevice device, Loader loader,
		IndexBuffer indexBuffer, VkQueue graphicsQueue, CommandBuffer commandBuffer, CommandPool commandPool, Buffer buffer);

};
