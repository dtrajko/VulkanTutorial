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
#include "engine/vulkan/Buffer.h"
#include "engine/vulkan/VertexBuffer.h"
#include "engine/vulkan/IndexBuffer.h"
#include "engine/vulkan/UniformBuffer.h"
#include "engine/vulkan/CommandBuffer.h"
#include "engine/vulkan/CommandPool.h"
#include "engine/vulkan/Image.h"
#include "engine/vulkan/ImageView.h"
#include "engine/vulkan/Sampler.h"
#include "engine/vulkan/ShaderModule.h"
#include "engine/vulkan/DescriptorSetLayout.h"
#include "engine/vulkan/DescriptorPool.h"
#include "engine/vulkan/DescriptorSet.h"
#include "engine/vulkan/SwapChain.h"


const int WIDTH = 1280;
const int HEIGHT = 720;


const int MAX_FRAMES_IN_FLIGHT = 2;


// Swapchain capability
const std::vector<const char*> deviceExtensions =
{
	VK_KHR_SWAPCHAIN_EXTENSION_NAME
};


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

	// Vulkan
	VkInstance instance;

	// Vulkan window surface
	VkSurfaceKHR surfaceKHR;

	// Vulkan physical devices
	VkPhysicalDevice hPhysicalDevice = VK_NULL_HANDLE;
	// std::optional<uint32_t> graphicsFamily;

	// Vulkan logical device
	VkDevice device;
	VkQueue graphicsQueue;

	// Presentation queue
	VkQueue presentQueue;

	// Swap chain
	std::vector<VkImageView> swapChainImageViews;

	// Framebuffers
	std::vector<VkFramebuffer> swapChainFramebuffers;
	bool framebufferResized = false; // used to recreate the swap chain

	// Render pass
	VkRenderPass renderPass;

	// Graphics pipeline
	VkPipelineLayout pipelineLayout;
	VkPipeline graphicsPipeline;

	// Command buffers
	std::vector<VkCommandBuffer> commandBuffers;

	// Semaphores (for synchronizing swap chain events)
	std::vector<VkSemaphore> imageAvailableSemaphores;
	std::vector<VkSemaphore> renderFinishedSemaphores;
	std::vector<VkFence> inFlightFences;
	std::vector<VkFence> imagesInFlight;
	size_t currentFrame = 0;

	// Textures
	VkImage textureImage;
	VkDeviceMemory textureImageMemory;

	// Depth resources
	VkImage depthImage;
	VkDeviceMemory depthImageMemory;
	VkImageView depthImageView;

	// Mipmaps
	uint32_t mipLevels;

	// Multisampling (MSAA)
	VkSampleCountFlagBits msaaSamples = VK_SAMPLE_COUNT_1_BIT;
	VkImage colorImage;
	VkDeviceMemory colorImageMemory;
	VkImageView colorImageView;

	// Refactoring
	Debug debug;
	Loader loader;
	PhysicalDevice physicalDevice;
	ValidationLayer validationLayer;
	Surface surface;
	Buffer buffer;
	VertexBuffer vertexBuffer;
	IndexBuffer indexBuffer;
	UniformBuffer uniformBuffer;
	CommandBuffer commandBuffer;
	CommandPool commandPool;
	Image image;
	ImageView imageView;
	Sampler sampler;
	ShaderModule shaderModule;
	DescriptorSetLayout descriptorSetLayout;
	DescriptorPool descriptorPool;
	DescriptorSet descriptorSet;
	SwapChain swapChain;


private:

	void initWindow();
	void initVulkan();
	void createInstance(bool enableValidationLayers);
	std::vector<const char*> getRequiredExtensions(bool enableValidationLayers);

	// physical devices
	void pickPhysicalDevice();
	bool isDeviceSuitable(VkPhysicalDevice device);
	bool checkDeviceExtensionSupport(VkPhysicalDevice device);
	int rateDeviceSuitability(VkPhysicalDevice device);

	// logical device
	void createLogicalDevice();

	// Swap chain support
	void createImageViews();
	void cleanupSwapChain(UniformBuffer uniformBuffer);
	void recreateSwapChain();

	// Render pass
	void createRenderPass();

	// Graphics pipeline
	void createGraphicsPipeline();

	// Framebuffers
	void createFramebuffers();

	// Command pools
	void createCommandBuffers();
	void transitionImageLayout(VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout, uint32_t mipLevels);

	// Semaphores (for synchronizing swap chain events)
	void createSyncObjects();

	// Uniform buffers
	void updateUniformBuffer(uint32_t currentImage, UniformBuffer uniformBuffer);

	// Texture mapping
	void createTextureImage(const char* texFilepath);

	// Depth resources
	void createDepthResources();
	VkFormat findSupportedFormat(const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features);
	VkFormat findDepthFormat();
	bool hasStencilComponent(VkFormat format);

	// Multisampling (MSAA)
	void createColorResources();

	void mainLoop();
	void drawFrame();

	void cleanup();

};
