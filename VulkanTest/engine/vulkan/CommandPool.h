#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include "PhysicalDevice.h"

#include <vector>

class Loader;
class SwapChain;
class VertexBuffer;
class IndexBuffer;
class DescriptorSet;


class CommandPool
{

public:

	// Command pool
	VkCommandPool commandPool;

	// Command buffers
	std::vector<VkCommandBuffer> commandBuffers;

public:

	void createCommandPool(PhysicalDevice physicalDevice, VkPhysicalDevice hPhysicalDevice, VkDevice device, VkSurfaceKHR surfaceKHR);

	void createCommandBuffers(VkDevice device, Loader loader, VkRenderPass renderPass, SwapChain swapChain,
		std::vector<VkFramebuffer> swapChainFramebuffers, VkPipeline graphicsPipeline, VkPipelineLayout pipelineLayout,
		VertexBuffer vertexBuffer, IndexBuffer indexBuffer, DescriptorSet descriptorSet);

};
