#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <vector>


class SwapChain;
class Image;


class Framebuffer
{
public:

	std::vector<VkFramebuffer> swapChainFramebuffers;

public:

	void createFramebuffers(VkDevice device, SwapChain* swapChain, Image image, VkRenderPass renderPass);

};
