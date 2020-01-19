#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEFAULT_ALIGNED_GENTYPES
#define GLM_ENABLE_EXPERIMENTAL
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/hash.hpp>

#include <iostream>
#include <fstream>
#include <stdexcept>
#include <algorithm>
#include <chrono>
#include <vector>
#include <cstring>
#include <cstdlib>
#include <cstdint>
#include <array>
#include <optional>
#include <set>
#include <unordered_map>
#include <functional>
#include <map>

#include "engine/Debug.h"
#include "engine/Loader.h"
#include "engine/vulkan/PhysicalDevice.h"
#include "engine/vulkan/ValidationLayer.h"
#include "engine/vulkan/Surface.h"
#include "engine/vulkan/Device.h"
#include "engine/vulkan/Buffer.h"
#include "engine/vulkan/VertexBuffer.h"
#include "engine/vulkan/IndexBuffer.h"
#include "engine/vulkan/UniformBuffer.h"
#include "engine/vulkan/CommandBuffer.h"
#include "engine/vulkan/CommandPool.h"
#include "engine/vulkan/Image.h"
#include "engine/vulkan/ImageView.h"
#include "engine/vulkan/Format.h"
#include "engine/vulkan/Sampler.h"
#include "engine/vulkan/ShaderModule.h"
#include "engine/vulkan/DescriptorSetLayout.h"
#include "engine/vulkan/DescriptorPool.h"
#include "engine/vulkan/DescriptorSet.h"
#include "engine/vulkan/SwapChain.h"
#include "engine/vulkan/Framebuffer.h"
#include "engine/vulkan/PipelineLayout.h"
#include "engine/vulkan/Instance.h"


const int WIDTH = 1280;
const int HEIGHT = 720;


const int MAX_FRAMES_IN_FLIGHT = 2;


#ifdef NDEBUG
	const bool enableValidationLayers = false;
#else
	const bool enableValidationLayers = true;
#endif // NDEBUG



class HelloTriangleApplication
{

public:

	static void framebufferResizeCallback(GLFWwindow* window, int width, int height);


public:

	void run();

private:

	// GLFW
	GLFWwindow* window;

	// Vulkan window surface
	VkSurfaceKHR surfaceKHR;

	// Vulkan physical devices
	VkPhysicalDevice hPhysicalDevice = VK_NULL_HANDLE;

	// Vulkan logical device
	VkDevice device;
	VkQueue graphicsQueue;

	// Presentation queue
	VkQueue presentQueue;

	// Framebuffers
	bool framebufferResized = false; // used to recreate the swap chain

	// Render pass
	VkRenderPass renderPass;

	// Graphics pipeline
	VkPipeline graphicsPipeline;

	// Semaphores (for synchronizing swap chain events)
	std::vector<VkSemaphore> imageAvailableSemaphores;
	std::vector<VkSemaphore> renderFinishedSemaphores;
	std::vector<VkFence> inFlightFences;
	std::vector<VkFence> imagesInFlight;
	size_t currentFrame = 0;

	// Refactoring
	Debug debug;
	Loader loader;
	PhysicalDevice physicalDevice;
	ValidationLayer validationLayer;
	Surface surface;
	Buffer* buffer;
	IndexBuffer indexBuffer;
	UniformBuffer uniformBuffer;
	CommandBuffer commandBuffer;
	Image image;
	ImageView imageView;
	ShaderModule shaderModule;
	DescriptorSetLayout descriptorSetLayout;
	DescriptorPool descriptorPool;
	DescriptorSet descriptorSet;
	SwapChain swapChain;
	Framebuffer framebuffer;
	Format format;
	Device logicalDevice;
	Sampler* textureSampler;
	VertexBuffer* vertexBuffer;
	Instance* instance;
	CommandPool* commandPool;
	PipelineLayout* pipelineLayout;


private:

	void initWindow();
	void initVulkan();

	// physical devices
	void pickPhysicalDevice(VkInstance hInstance, PhysicalDevice physicalDevice, VkPhysicalDevice& hPhysicalDevice,
		VkSurfaceKHR surfaceKHR, SwapChain swapChain, VkSampleCountFlagBits& msaaSamples);

	// Swap chain support
	void cleanupSwapChain(UniformBuffer uniformBuffer);
	void recreateSwapChain();

	// Render pass
	void createRenderPass();

	// Graphics pipeline
	void createGraphicsPipeline();

	// Semaphores (for synchronizing swap chain events)
	void createSyncObjects();

	// Uniform buffers
	void updateUniformBuffer(uint32_t currentImage, UniformBuffer uniformBuffer);

	void mainLoop();
	void drawFrame();

	void cleanup();

};
