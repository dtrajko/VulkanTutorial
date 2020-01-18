#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>


class SwapChain;


class DescriptorPool
{
public:

	VkDescriptorPool descriptorPool;

	void createDescriptorPool(VkDevice device, SwapChain swapChain);


};
