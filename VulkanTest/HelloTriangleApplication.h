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
#include "engine/Buffer.h"
#include "engine/PhysicalDevice.h"
#include "engine/Surface.h"
#include "engine/CommandBuffer.h"
#include "engine/CommandPool.h"
#include "engine/Sampler.h"


const int WIDTH = 1280;
const int HEIGHT = 720;


const int MAX_FRAMES_IN_FLIGHT = 2;

const std::vector<const char*> validationLayers =
{
	"VK_LAYER_KHRONOS_validation"
};

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


struct SwapChainSupportDetails
{
	VkSurfaceCapabilitiesKHR capabilities;
	std::vector<VkSurfaceFormatKHR> formats;
	std::vector<VkPresentModeKHR> presentModes;
};

struct UniformBufferObject
{
	alignas(16) glm::mat4 model;
	alignas(16) glm::mat4 view;
	alignas(16) glm::mat4 proj;
};

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
	VkDebugUtilsMessengerEXT debugMessenger;

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
	VkSwapchainKHR swapChain;
	std::vector<VkImage> swapChainImages;
	VkFormat swapChainImageFormat;
	VkExtent2D swapChainExtent;
	std::vector<VkImageView> swapChainImageViews;

	// Framebuffers
	std::vector<VkFramebuffer> swapChainFramebuffers;
	bool framebufferResized = false; // used to recreate the swap chain

	// Render pass
	VkRenderPass renderPass;

	// Descriptors
	VkDescriptorSetLayout descriptorSetLayout;
	VkDescriptorPool descriptorPool;
	std::vector<VkDescriptorSet> descriptorSets;

	// Graphics pipeline
	VkPipelineLayout pipelineLayout;
	VkPipeline graphicsPipeline;

	// Vertex buffer
	VkBuffer vertexBuffer;
	VkDeviceMemory vertexBufferMemory;

	// Index buffer
	VkBuffer indexBuffer;
	VkDeviceMemory indexBufferMemory;

	// Uniform buffers
	std::vector<VkBuffer> uniformBuffers;
	std::vector<VkDeviceMemory> uniformBuffersMemory;

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
	VkImageView textureImageView;

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
	Loader loader;
	Buffer buffer;
	PhysicalDevice physicalDevice;
	Surface surface;
	CommandBuffer commandBuffer;
	CommandPool commandPool;
	Sampler sampler;


private:

	void initWindow();
	void initVulkan();
	void createInstance();
	std::vector<const char*> getRequiredExtensions();

	// debug / validation layers
	void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo);
	void setupDebugMessenger();
	bool checkValidationLayerSupport();

	// physical devices
	void pickPhysicalDevice();
	bool isDeviceSuitable(VkPhysicalDevice device);
	bool checkDeviceExtensionSupport(VkPhysicalDevice device);
	int rateDeviceSuitability(VkPhysicalDevice device);
	void printDeviceProperties(
		VkPhysicalDeviceProperties deviceProperties,
		VkPhysicalDeviceFeatures deviceFeatures,
		int score);

	// logical device
	void createLogicalDevice();

	// Window surface
	void createSurface();

	// Swap chain support
	SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device);
	void printSwapChainSupport(bool swapChainAdequate, SwapChainSupportDetails swapChainSupport);
	VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
	VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);
	void createSwapChain();
	void createImageViews();
	void cleanupSwapChain();
	void recreateSwapChain();

	// Render pass
	void createRenderPass();

	// Graphics pipeline
	void createGraphicsPipeline();
	VkShaderModule createShaderModule(const std::vector<char>& code);

	// Framebuffers
	void createFramebuffers();

	// Command pools
	void createCommandBuffers();
	void transitionImageLayout(VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout, uint32_t mipLevels);

	// Semaphores (for synchronizing swap chain events)
	void createSyncObjects();

	// Vertex buffer
	void createVertexBuffer();
	void copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);
	void copyBufferToImage(VkBuffer buffer, VkImage image, uint32_t width, uint32_t height);

	// Index buffer
	void createIndexBuffer();

	// Uniform buffers
	void createUniformBuffers();
	void updateUniformBuffer(uint32_t currentImage);

	// Descriptors
	void createDescriptorSetLayout();
	void createDescriptorPool();
	void createDescriptorSets();

	// Texture mapping
	void createTextureImage(const char* texFilepath);
	void createImage(uint32_t width, uint32_t height, uint32_t mipLevels, VkSampleCountFlagBits numSamples,
		VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties,
		VkImage& image, VkDeviceMemory& imageMemory);
	void createTextureImageView();
	VkImageView createImageView(VkImage image, VkFormat format, VkImageAspectFlags aspectFlags, uint32_t mipLevels);
	void createTextureSampler();
	void generateMipmaps(VkImage image, VkFormat imageFormat, int32_t texWidth, int32_t texHeight, uint32_t mipLevels);

	// Depth resources
	void createDepthResources();
	VkFormat findSupportedFormat(const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features);
	VkFormat findDepthFormat();
	bool hasStencilComponent(VkFormat format);

	// Multisampling (MSAA)
	VkSampleCountFlagBits getMaxUsableSampleCount();
	void createColorResources();

	void mainLoop();
	void drawFrame();

	void cleanup();

};
