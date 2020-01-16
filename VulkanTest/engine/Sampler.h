#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>


class Sampler
{

public:

	VkSampler textureSampler;

	void createTextureSampler(VkDevice device, uint32_t mipLevels);

};
