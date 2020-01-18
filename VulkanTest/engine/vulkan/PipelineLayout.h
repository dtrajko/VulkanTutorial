#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>


class DescriptorSetLayout;


class PipelineLayout
{

public:
	
	VkPipelineLayout pipelineLayout;


public:

	void createPipelineLayout(VkDevice device, DescriptorSetLayout descriptorSetLayout);

};
