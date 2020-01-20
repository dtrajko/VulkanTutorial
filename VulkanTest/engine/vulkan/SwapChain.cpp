#include "SwapChain.h"

#include "ImageView.h"

#include <stdexcept>


SwapChain::SwapChain(GLFWwindow* window, PhysicalDevice* physicalDevice, VkDevice device, Surface* surface)
{
	createSwapChain(window, physicalDevice, device, surface);
}

void SwapChain::createSwapChain(GLFWwindow* window, PhysicalDevice* physicalDevice, VkDevice device, Surface* surface)
{
	SwapChainSupportDetails swapChainSupport = querySwapChainSupport(physicalDevice->m_Device, surface->m_surfaceKHR);

	VkSurfaceFormatKHR surfaceFormat = surface->chooseSwapSurfaceFormat(swapChainSupport.formats);
	VkPresentModeKHR presentMode = surface->chooseSwapPresentMode(swapChainSupport.presentModes);
	VkExtent2D extent = surface->chooseSwapExtent(window, swapChainSupport.capabilities);

	uint32_t imageCount = swapChainSupport.capabilities.minImageCount + 1;

	if (swapChainSupport.capabilities.maxImageCount > 0 && imageCount > swapChainSupport.capabilities.maxImageCount)
	{
		imageCount = swapChainSupport.capabilities.maxImageCount;
	}

	VkSwapchainCreateInfoKHR createInfo = {};
	createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
	createInfo.surface = surface->m_surfaceKHR;
	createInfo.minImageCount = imageCount;
	createInfo.imageFormat = surfaceFormat.format;
	createInfo.imageColorSpace = surfaceFormat.colorSpace;
	createInfo.imageExtent = extent;
	createInfo.imageArrayLayers = 1;
	createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

	QueueFamilyIndices indices = physicalDevice->findQueueFamilies(physicalDevice->m_Device, surface->m_surfaceKHR);
	uint32_t queueFamilyIndices[] = { indices.graphicsFamily.value(), indices.presentFamily.value() };

	if (indices.graphicsFamily != indices.presentFamily)
	{
		createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
		createInfo.queueFamilyIndexCount = 2;
		createInfo.pQueueFamilyIndices = queueFamilyIndices;
	}
	else
	{
		createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
	}

	createInfo.preTransform = swapChainSupport.capabilities.currentTransform;
	createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
	createInfo.presentMode = presentMode;
	createInfo.clipped = VK_TRUE;

	if (vkCreateSwapchainKHR(device, &createInfo, nullptr, &swapChain) != VK_SUCCESS)
	{
		throw std::runtime_error("Failed to create the swap chain.");
	}

	// Retrieving the handles of swap chain images
	vkGetSwapchainImagesKHR(device, swapChain, &imageCount, nullptr);
	swapChainImages.resize(imageCount);
	vkGetSwapchainImagesKHR(device, swapChain, &imageCount, swapChainImages.data());

	swapChainImageFormat = surfaceFormat.format;
	swapChainExtent = extent;
}

SwapChainSupportDetails SwapChain::querySwapChainSupport(VkPhysicalDevice device, VkSurfaceKHR surfaceKHR)
{
	SwapChainSupportDetails details;

	// Step 1
	vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, surfaceKHR, &details.capabilities);

	// Step 2
	uint32_t formatCount;
	vkGetPhysicalDeviceSurfaceFormatsKHR(device, surfaceKHR, &formatCount, nullptr);

	if (formatCount != 0)
	{
		details.formats.resize(formatCount);
		vkGetPhysicalDeviceSurfaceFormatsKHR(device, surfaceKHR, &formatCount, details.formats.data());
	}

	// Step 3
	uint32_t presentModeCount;
	vkGetPhysicalDeviceSurfacePresentModesKHR(device, surfaceKHR, &presentModeCount, nullptr);

	if (presentModeCount != 0)
	{
		details.presentModes.resize(presentModeCount);
		vkGetPhysicalDeviceSurfacePresentModesKHR(device, surfaceKHR, &presentModeCount, details.presentModes.data());
	}

	return details;
}

void SwapChain::createImageViews(VkDevice device, ImageView imageView)
{
	swapChainImageViews.resize(swapChainImages.size());

	for (size_t i = 0; i < swapChainImages.size(); i++)
	{
		swapChainImageViews[i] = imageView.createImageView(device, swapChainImages[i], swapChainImageFormat, VK_IMAGE_ASPECT_COLOR_BIT, 1);
	}
}
