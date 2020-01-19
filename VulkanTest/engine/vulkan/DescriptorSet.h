#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <vector>


class UniformBuffer;
class SwapChain;
class DescriptorSetLayout;
class DescriptorPool;
class ImageView;
class Sampler;


class DescriptorSet
{
public:

	// Descriptors
	std::vector<VkDescriptorSet> descriptorSets;

	void createDescriptorSets(VkDevice device, UniformBuffer uniformBuffer, SwapChain swapChain,
		DescriptorSetLayout descriptorSetLayout, DescriptorPool descriptorPool, ImageView imageView, Sampler* sampler);


};
