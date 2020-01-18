#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>

#include <vector>


struct UniformBufferObject
{
	alignas(16) glm::mat4 model;
	alignas(16) glm::mat4 view;
	alignas(16) glm::mat4 proj;
};


class SwapChain;
class Buffer;


class UniformBuffer
{

public:

	std::vector<VkBuffer> uniformBuffers;
	std::vector<VkDeviceMemory> uniformBuffersMemory;

	void createUniformBuffers(VkDevice device, VkPhysicalDevice hPhysicalDevice, SwapChain swapChain, Buffer buffer);

};