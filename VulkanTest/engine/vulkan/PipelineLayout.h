#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>


class DescriptorSetLayout;


class PipelineLayout
{

public:
	
	VkPipelineLayout pipelineLayout;
	VkDevice m_device;


public:

	PipelineLayout(VkDevice device, DescriptorSetLayout descriptorSetLayout);
	~PipelineLayout();

};
