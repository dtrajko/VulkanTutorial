#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include "PhysicalDevice.h"


class CommandPool
{

public:

	// Command pool
	VkCommandPool commandPool;

	void createCommandPool(PhysicalDevice physicalDevice, VkPhysicalDevice hPhysicalDevice, VkDevice device, VkSurfaceKHR surfaceKHR);

};
