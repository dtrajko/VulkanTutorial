#include "PhysicalDevice.h"

#include "../Print.h"
#include "SwapChain.h"

#include <stdexcept>
#include <string>
#include <set>


bool PhysicalDevice::isDeviceSuitable(VkPhysicalDevice hPhysicalDevice, VkSurfaceKHR surfaceKHR, SwapChain swapChain)
{
	QueueFamilyIndices indices = findQueueFamilies(hPhysicalDevice, surfaceKHR);

	bool extensionsSupported = checkDeviceExtensionSupport(hPhysicalDevice);

	bool swapChainAdequate = false;
	SwapChainSupportDetails swapChainSupport;

	if (extensionsSupported)
	{
		swapChainSupport = swapChain.querySwapChainSupport(hPhysicalDevice, surfaceKHR);
		swapChainAdequate = !swapChainSupport.formats.empty() && !swapChainSupport.presentModes.empty();
	}

	VkPhysicalDeviceFeatures supportedFeatures;
	vkGetPhysicalDeviceFeatures(hPhysicalDevice, &supportedFeatures);

	Print::printSwapChainSupport(swapChainAdequate, swapChainSupport);

	return indices.isComplete() && extensionsSupported && swapChainAdequate && supportedFeatures.samplerAnisotropy;
}

bool PhysicalDevice::checkDeviceExtensionSupport(VkPhysicalDevice hPhysicalDevice)
{
	uint32_t extensionCount;
	vkEnumerateDeviceExtensionProperties(hPhysicalDevice, nullptr, &extensionCount, nullptr);

	std::vector<VkExtensionProperties> availableExtensions(extensionCount);
	vkEnumerateDeviceExtensionProperties(hPhysicalDevice, nullptr, &extensionCount, availableExtensions.data());

	std::set<std::string> requiredExtensions(deviceExtensions.begin(), deviceExtensions.end());

	std::cout << std::endl;
	std::cout << "Device Extension Support: " << std::endl;

	for (const auto& extension : availableExtensions)
	{
		requiredExtensions.erase(extension.extensionName);
		std::cout << "\t" << "extensionName: " << extension.extensionName << std::endl;
	}

	return requiredExtensions.empty();
}

int PhysicalDevice::rateDeviceSuitability(VkPhysicalDevice device)
{
	VkPhysicalDeviceProperties deviceProperties;
	VkPhysicalDeviceFeatures deviceFeatures;
	vkGetPhysicalDeviceProperties(device, &deviceProperties);
	vkGetPhysicalDeviceFeatures(device, &deviceFeatures);

	int score = 0;

	// Discrete GPUs have a significant performance advantage
	if (deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU)
	{
		score += 1000;
	}

	// Maximum possible size of textures affects graphics quality
	score += deviceProperties.limits.maxImageDimension2D;

	// Application can't function without geometry shaders
	if (!deviceFeatures.geometryShader)
	{
		score = 0;
	}

	Print::printDeviceProperties(deviceProperties, deviceFeatures, score);

	return score;
}

uint32_t PhysicalDevice::findMemoryType(VkPhysicalDevice physicalDevice, uint32_t typeFilter, VkMemoryPropertyFlags properties)
{
	VkPhysicalDeviceMemoryProperties memProperties;
	vkGetPhysicalDeviceMemoryProperties(physicalDevice, &memProperties);

	for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++)
	{
		if ((typeFilter & (1 << i)) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties)
		{
			return i;
		}
	}

	throw std::runtime_error("Failed to find suitable memory type!");
}

QueueFamilyIndices PhysicalDevice::findQueueFamilies(VkPhysicalDevice physicalDevice, VkSurfaceKHR& surface)
{
	// Logic to find graphics queue family
	QueueFamilyIndices indices = {};
	uint32_t queueFamilyCount = 0;
	vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyCount, nullptr);
	std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
	vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyCount, queueFamilies.data());

	int i = 0;
	for (const auto& queueFamily : queueFamilies)
	{
		if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT)
		{
			indices.graphicsFamily = i;
		}

		VkBool32 presentSupport = false;
		vkGetPhysicalDeviceSurfaceSupportKHR(physicalDevice, i, surface, &presentSupport);

		if (presentSupport)
		{
			indices.presentFamily = i;
		}

		if (indices.isComplete())
		{
			break;
		}

		i++;
	}

	return indices;
}

VkSampleCountFlagBits PhysicalDevice::getMaxUsableSampleCount(VkPhysicalDevice hPhysicalDevice)
{
	VkPhysicalDeviceProperties physicalDeviceProperties;
	vkGetPhysicalDeviceProperties(hPhysicalDevice, &physicalDeviceProperties);

	VkSampleCountFlags counts =
		physicalDeviceProperties.limits.framebufferColorSampleCounts &
		physicalDeviceProperties.limits.framebufferDepthSampleCounts;

	if (counts & VK_SAMPLE_COUNT_64_BIT) { return VK_SAMPLE_COUNT_64_BIT; }
	if (counts & VK_SAMPLE_COUNT_32_BIT) { return VK_SAMPLE_COUNT_32_BIT; }
	if (counts & VK_SAMPLE_COUNT_16_BIT) { return VK_SAMPLE_COUNT_16_BIT; }
	if (counts & VK_SAMPLE_COUNT_8_BIT) { return VK_SAMPLE_COUNT_8_BIT; }
	if (counts & VK_SAMPLE_COUNT_4_BIT) { return VK_SAMPLE_COUNT_4_BIT; }
	if (counts & VK_SAMPLE_COUNT_2_BIT) { return VK_SAMPLE_COUNT_2_BIT; }

	return VK_SAMPLE_COUNT_1_BIT;
}