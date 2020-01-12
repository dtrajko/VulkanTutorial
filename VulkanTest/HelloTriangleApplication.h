#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEFAULT_ALIGNED_GENTYPES
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <iostream>
#include <stdexcept>
#include <functional>
#include <cstdlib>
#include <vector>
#include <cstring>
#include <map>
#include <optional>
#include <set>
#include <cstdint>
#include <algorithm>
#include <fstream>
#include <array>
#include <chrono>


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


struct QueueFamilyIndices
{
	std::optional<uint32_t> graphicsFamily;
	std::optional<uint32_t> presentFamily;

	bool isComplete()
	{
		return graphicsFamily.has_value() && presentFamily.has_value();
	}
};

struct SwapChainSupportDetails
{
	VkSurfaceCapabilitiesKHR capabilities;
	std::vector<VkSurfaceFormatKHR> formats;
	std::vector<VkPresentModeKHR> presentModes;
};

struct Vertex
{
	glm::vec2 pos;
	glm::vec3 color;

	static VkVertexInputBindingDescription getBindingDescription()
	{
		VkVertexInputBindingDescription bindingDescription = {};
		bindingDescription.binding = 0;
		bindingDescription.stride = sizeof(Vertex);
		bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
		return bindingDescription;
	}

	static std::array<VkVertexInputAttributeDescription, 2> getAttributeDescriptions() {
		std::array<VkVertexInputAttributeDescription, 2> attributeDescriptions = {};

		attributeDescriptions[0].binding = 0;
		attributeDescriptions[0].location = 0;
		attributeDescriptions[0].format = VK_FORMAT_R32G32_SFLOAT;
		attributeDescriptions[0].offset = offsetof(Vertex, pos);

		attributeDescriptions[1].binding = 0;
		attributeDescriptions[1].location = 1;
		attributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
		attributeDescriptions[1].offset = offsetof(Vertex, color);

		return attributeDescriptions;
	}
};

struct UniformBufferObject
{
	alignas(16) glm::mat4 model;
	alignas(16) glm::mat4 view;
	alignas(16) glm::mat4 proj;
};

const std::vector<Vertex> vertices_triangle =
{
	{ { 0.0f, -0.5f }, { 1.0f, 0.0f, 0.0f } },
	{ { 0.5f,  0.5f }, { 0.0f, 1.0f, 0.0f } },
	{ {-0.5f,  0.5f }, { 0.0f, 0.0f, 1.0f } },
};

const std::vector<Vertex> vertices = {
	{ {-0.5f, -0.5f}, { 1.0f, 0.0f, 0.0f } },
	{ { 0.5f, -0.5f}, { 0.0f, 1.0f, 0.0f } },
	{ { 0.5f,  0.5f}, { 0.0f, 0.0f, 1.0f } },
	{ {-0.5f,  0.5f}, { 1.0f, 1.0f, 1.0f } }
};

const std::vector<uint16_t> indices = {
	0, 1, 2, 2, 3, 0
};


class HelloTriangleApplication
{

public:

	static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
		VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
		VkDebugUtilsMessageTypeFlagsEXT messageType,
		const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
		void* pUserData);

	static VkResult CreateDebugUtilsMessengerEXT(
		VkInstance instance,
		const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo,
		const VkAllocationCallbacks* pAllocator,
		VkDebugUtilsMessengerEXT* pDebugMessenger);

	static void DestroyDebugUtilsMessengerEXT(
		VkInstance instance,
		VkDebugUtilsMessengerEXT debugMessenger,
		const VkAllocationCallbacks* pAllocator);

	static std::vector<char> readFile(const std::string& filename);

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
	VkSurfaceKHR surface;

	// Vulkan physical devices
	VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
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

	// Command pool
	VkCommandPool commandPool;

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
	VkSampler textureSampler;


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
	QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device);

	// logical device
	void createLogicalDevice();

	// Window surface
	void createSurface();

	// Swap chain support
	SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device);
	void printSwapChainSupport(bool swapChainAdequate, SwapChainSupportDetails swapChainSupport);
	VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
	VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);
	VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);
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
	void createCommandPool();
	void createCommandBuffers();
	VkCommandBuffer beginSingleTimeCommands();
	void endSingleTimeCommands(VkCommandBuffer commandBuffer);
	void transitionImageLayout(VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout);

	// Semaphores (for synchronizing swap chain events)
	void createSyncObjects();

	// Vertex buffer
	void createVertexBuffer();
	uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);
	void createBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties,
		VkBuffer& buffer, VkDeviceMemory& bufferMemory);
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
	void createTextureImage();
	void createImage(uint32_t width, uint32_t height, VkFormat format, VkImageTiling tiling,
		VkImageUsageFlags usage, VkMemoryPropertyFlags properties, VkImage& image, VkDeviceMemory& imageMemory);
	void createTextureImageView();
	VkImageView createImageView(VkImage image, VkFormat format);
	void createTextureSampler();

	void mainLoop();
	void drawFrame();

	void cleanup();

};
