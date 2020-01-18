#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include "PhysicalDevice.h"

#include <vector>


class CommandBuffer;
class CommandPool;
class SwapChain;
class ImageView;
class Format;


class Image
{
public:

	// Multisampling (MSAA)
	VkSampleCountFlagBits msaaSamples = VK_SAMPLE_COUNT_1_BIT;
	VkImage colorImage;
	VkDeviceMemory colorImageMemory;
	VkImageView colorImageView;

	// Depth resources
	VkImage depthImage;
	VkDeviceMemory depthImageMemory;
	VkImageView depthImageView;


public:

	void createImage(VkDevice device, PhysicalDevice physicalDevice, VkPhysicalDevice hPhysicalDevice,
		uint32_t width, uint32_t height, uint32_t mipLevels, VkSampleCountFlagBits numSamples, VkFormat format, VkImageTiling tiling,
		VkImageUsageFlags usage, VkMemoryPropertyFlags properties, VkImage& image, VkDeviceMemory& imageMemory);

	void transitionImageLayout(VkDevice device, CommandBuffer commandBuffer, CommandPool commandPool, VkImage image, VkFormat imageFormat,
		VkImageLayout oldLayout, VkImageLayout newLayout, uint32_t mipLevels, Format format, VkQueue graphicsQueue);

	// Multisampling (MSAA)
	void createColorResources(VkDevice device, PhysicalDevice physicalDevice, VkPhysicalDevice hPhysicalDevice, SwapChain swapChain, ImageView imageView);

	// Depth resources
	void createDepthResources(VkDevice device, PhysicalDevice physicalDevice, VkPhysicalDevice hPhysicalDevice, SwapChain swapChain, ImageView imageView,
		CommandBuffer commandBuffer, CommandPool commandPool, Format format, VkQueue graphicsQueue);

	VkFormat findSupportedFormat(VkPhysicalDevice hPhysicalDevice, const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features);
	VkFormat findDepthFormat(VkPhysicalDevice hPhysicalDevice);

	void generateMipmaps(VkPhysicalDevice hPhysicalDevice, VkDevice device, CommandBuffer commandBuffer, CommandPool commandPool,
		VkQueue graphicsQueue, VkImage image, VkFormat imageFormat, int32_t texWidth, int32_t texHeight, uint32_t mipLevels);

};
