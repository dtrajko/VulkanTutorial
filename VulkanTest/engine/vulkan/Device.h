#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>


class PhysicalDevice;


class Device
{
public:

	// logical device
	void createLogicalDevice(PhysicalDevice physicalDevice, VkPhysicalDevice& hPhysicalDevice, VkDevice& device,
		VkSurfaceKHR surfaceKHR, bool enableValidationLayers, VkQueue& graphicsQueue, VkQueue& presentQueue);

};
