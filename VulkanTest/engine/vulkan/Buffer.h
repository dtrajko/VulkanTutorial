#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include "PhysicalDevice.h"


class Buffer
{

public:

	VkBuffer m_Buffer;
	VkDeviceMemory m_Memory;
	VkDevice m_Device;

public:

	Buffer(VkDevice device, VkPhysicalDevice hPhysicalDevice, VkDeviceSize size, VkBufferUsageFlags usage,
		VkMemoryPropertyFlags properties);
	~Buffer();

private:
	PhysicalDevice physicalDevice;

};
