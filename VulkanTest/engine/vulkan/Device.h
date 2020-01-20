#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>


class PhysicalDevice;


class Device
{

public:

	VkDevice m_Device;

public:

	Device(PhysicalDevice* physicalDevice, VkSurfaceKHR surfaceKHR, bool enableValidationLayers, VkQueue& graphicsQueue, VkQueue& presentQueue);

	~Device();

};
