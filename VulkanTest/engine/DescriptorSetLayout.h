#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>


class DescriptorSetLayout
{

public:

	VkDescriptorSetLayout descriptorSetLayout;

	void createDescriptorSetLayout(VkDevice device);

};
