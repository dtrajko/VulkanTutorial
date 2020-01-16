#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>


class PhysicalDevice
{

public:

	uint32_t findMemoryType(VkPhysicalDevice physicalDevice, uint32_t typeFilter, VkMemoryPropertyFlags properties);


};
