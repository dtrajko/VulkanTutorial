#include "ImageView.h"

#include <stdexcept>


VkImageView ImageView::createImageView(VkDevice device, VkImage image, VkFormat format, VkImageAspectFlags aspectFlags, uint32_t mipLevels)
{
	VkImageViewCreateInfo viewInfo = {};
	viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
	viewInfo.image = image;
	viewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
	viewInfo.format = format;
	viewInfo.subresourceRange.aspectMask = aspectFlags;
	viewInfo.subresourceRange.baseMipLevel = 0;
	viewInfo.subresourceRange.levelCount = mipLevels;
	viewInfo.subresourceRange.baseArrayLayer = 0;
	viewInfo.subresourceRange.layerCount = 1;

	VkImageView imageView;
	if (vkCreateImageView(device, &viewInfo, nullptr, &imageView) != VK_SUCCESS)
	{
		throw std::runtime_error("Failed to create image views!");
	}

	return imageView;
}

void ImageView::createTextureImageView(VkDevice device, VkImage image, uint32_t mipLevels)
{
	m_ImageView = createImageView(device, image, VK_FORMAT_R8G8B8A8_UNORM, VK_IMAGE_ASPECT_COLOR_BIT, mipLevels);
}
