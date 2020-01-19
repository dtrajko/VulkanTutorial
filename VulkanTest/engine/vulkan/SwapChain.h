#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <vector>

#include "PhysicalDevice.h"
#include "Surface.h"


struct SwapChainSupportDetails
{
	VkSurfaceCapabilitiesKHR capabilities;
	std::vector<VkSurfaceFormatKHR> formats;
	std::vector<VkPresentModeKHR> presentModes;
};


class ImageView;


class SwapChain
{
public:

	VkSwapchainKHR swapChain;
	std::vector<VkImage> swapChainImages;
	VkFormat swapChainImageFormat;
	VkExtent2D swapChainExtent;
	std::vector<VkImageView> swapChainImageViews;

	void createSwapChain(GLFWwindow* window, VkPhysicalDevice hPhysicalDevice, PhysicalDevice physicalDevice,
		VkDevice device, Surface* surface);

	SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device, VkSurfaceKHR surfaceKHR);

	void createImageViews(VkDevice device, ImageView imageView);

};
