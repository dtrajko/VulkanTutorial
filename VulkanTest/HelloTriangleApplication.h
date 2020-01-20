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
#include <chrono>

#include "engine/Debug.h"
#include "engine/Loader.h"
#include "engine/Window.h"
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
#include "engine/vulkan/RenderPass.h"
#include "engine/vulkan/GraphicsPipeline.h"


const int MAX_FRAMES_IN_FLIGHT = 2;

#ifdef NDEBUG
	const bool enableValidationLayers = false;
#else
	const bool enableValidationLayers = true;
#endif // NDEBUG


class HelloTriangleApplication
{

public:

	void run();


private:

	// Vulkan logical device
	VkDevice device;
	VkQueue graphicsQueue;

	// Presentation queue
	VkQueue presentQueue;

	// Semaphores (for synchronizing swap chain events)
	std::vector<VkSemaphore> imageAvailableSemaphores;
	std::vector<VkSemaphore> renderFinishedSemaphores;
	std::vector<VkFence> inFlightFences;
	std::vector<VkFence> imagesInFlight;
	size_t currentFrame = 0;

	// Refactoring
	ValidationLayer validationLayer;
	Image image;
	ImageView imageView;
	ShaderModule shaderModule;
	DescriptorPool descriptorPool;
	DescriptorSet descriptorSet;
	SwapChain swapChain;
	Framebuffer framebuffer;
	Format format;
	Device logicalDevice;
	CommandBuffer commandBuffer;

	// RAII
	Window* window;
	Debug* debug;
	Instance* instance;
	Surface* surface;
	Loader* loader;
	PhysicalDevice* physicalDevice;
	Buffer* buffer;
	VertexBuffer* vertexBuffer;
	IndexBuffer* indexBuffer;
	UniformBuffer uniformBuffer;
	Sampler* textureSampler;
	CommandPool* commandPool;
	DescriptorSetLayout descriptorSetLayout;
	RenderPass* renderPass;
	GraphicsPipeline* graphicsPipeline;


private:

	void initVulkan();

	// Swap chain support
	void cleanupSwapChain(UniformBuffer uniformBuffer);
	void recreateSwapChain();

	// Semaphores (for synchronizing swap chain events)
	void createSyncObjects();

	// Uniform buffers
	void updateUniformBuffer(uint32_t currentImage, UniformBuffer uniformBuffer);

	void mainLoop();
	void drawFrame();

	void cleanup();

};
