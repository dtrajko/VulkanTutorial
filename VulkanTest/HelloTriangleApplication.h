#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <iostream>
#include <stdexcept>
#include <functional>
#include <cstdlib>
#include <vector>
#include <cstring>
#include <map>


const int WIDTH = 1280;
const int HEIGHT = 720;

const std::vector<const char*> validationLayers =
{
	"VK_LAYER_KHRONOS_validation"
};

#ifdef NDEBUG
	const bool enableValidationLayers = false;
#else
	const bool enableValidationLayers = true;
#endif // NDEBUG


struct QueueFamilyIndices
{
	uint32_t graphicsFamily;

	bool isComplete()
	{
		return graphicsFamily > -1;
	}
};

struct QueueFamilyIndicesII {
	// std::optional<uint32_t> graphicsFamily;

	bool isComplete() {
		// return graphicsFamily.has_value();
		return false;
	}
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

	void run();

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
	bool isDeviceSuitableII(VkPhysicalDevice device);
	int rateDeviceSuitability(VkPhysicalDevice device);
	void printDeviceProperties(
		VkPhysicalDeviceProperties deviceProperties,
		VkPhysicalDeviceFeatures deviceFeatures,
		int score);
	QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device);

	void mainLoop();
	void cleanup();

private:

	// GLFW
	GLFWwindow* window;

	// Vulkan
	VkInstance instance;
	VkDebugUtilsMessengerEXT debugMessenger;
	VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;

};
