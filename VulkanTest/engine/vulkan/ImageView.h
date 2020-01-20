#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>


class ImageView
{

public:

	VkImageView createImageView(VkDevice device, VkImage image, VkFormat format, VkImageAspectFlags aspectFlags, uint32_t mipLevels);

};
