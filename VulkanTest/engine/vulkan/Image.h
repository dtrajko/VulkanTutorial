#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include "PhysicalDevice.h"


class CommandBuffer;
class CommandPool;


class Image
{
public:

	void createImage(VkDevice device, PhysicalDevice physicalDevice, VkPhysicalDevice hPhysicalDevice,
		uint32_t width, uint32_t height, uint32_t mipLevels, VkSampleCountFlagBits numSamples, VkFormat format, VkImageTiling tiling,
		VkImageUsageFlags usage, VkMemoryPropertyFlags properties, VkImage& image, VkDeviceMemory& imageMemory);

	void generateMipmaps(VkPhysicalDevice hPhysicalDevice, VkDevice device, CommandBuffer commandBuffer, CommandPool commandPool,
		VkQueue graphicsQueue, VkImage image, VkFormat imageFormat, int32_t texWidth, int32_t texHeight, uint32_t mipLevels);

};
