#include "HelloTriangleApplication.h"
#include "engine/Print.h"


void HelloTriangleApplication::run()
{
	window = new Window();
	initVulkan();
	mainLoop();
	cleanup();
}

void HelloTriangleApplication::initVulkan()
{
	instance = new Instance(enableValidationLayers, validationLayers, validationLayer);
	debug = new Debug(instance->hInstance, enableValidationLayers);
	surface = new Surface(instance->hInstance, window->m_Window);
	loader = new Loader();
	physicalDevice = new PhysicalDevice(instance->hInstance, surface->m_surfaceKHR, swapChain, image.msaaSamples);
	logicalDevice.createLogicalDevice(physicalDevice, device, surface->m_surfaceKHR, enableValidationLayers, graphicsQueue, presentQueue);
	swapChain.createSwapChain(window->m_Window, physicalDevice, device, surface);
	swapChain.createImageViews(device, imageView);
	renderPass = new RenderPass(physicalDevice, device, swapChain, image);
	descriptorSetLayout.createDescriptorSetLayout(device);
	// descriptorSetLayout = new DescriptorSetLayout(device);
	graphicsPipeline = new GraphicsPipeline(device, shaderModule, swapChain, image, descriptorSetLayout, renderPass);
	commandPool = new CommandPool(physicalDevice, device, surface->m_surfaceKHR);
	image.createColorResources(device, physicalDevice, swapChain, imageView);
	image.createDepthResources(device, physicalDevice, swapChain, imageView, commandBuffer, commandPool, format, graphicsQueue);
	framebuffer.createFramebuffers(device, swapChain, image.colorImageView, image.depthImageView, renderPass->m_RenderPass);
	image.createTextureImage(loader->TEXTURE_PATH.c_str(), device, physicalDevice, commandBuffer, commandPool, format, graphicsQueue);
	imageView.createTextureImageView(device, image.textureImage, image.mipLevels);
	textureSampler = new Sampler(device, image.mipLevels);
	loader->loadModel();
	vertexBuffer = new VertexBuffer(physicalDevice, device, loader, indexBuffer, graphicsQueue, commandBuffer, commandPool);
	indexBuffer = new IndexBuffer(physicalDevice, device, loader, buffer, graphicsQueue, commandBuffer, commandPool);
	uniformBuffer.createUniformBuffers(physicalDevice, device, swapChain);
 	descriptorPool.createDescriptorPool(device, swapChain);
	descriptorSet.createDescriptorSets(device, uniformBuffer, swapChain, descriptorSetLayout, descriptorPool, imageView, textureSampler);
	commandPool->createCommandBuffers(device, loader, renderPass->m_RenderPass, swapChain, framebuffer.swapChainFramebuffers,
		graphicsPipeline->m_Pipeline, graphicsPipeline->m_PipelineLayout->m_PipelineLayout, vertexBuffer, indexBuffer, descriptorSet);
	createSyncObjects();
}

void HelloTriangleApplication::updateUniformBuffer(uint32_t currentImage, UniformBuffer uniformBuffer)
{
	static auto startTime = std::chrono::high_resolution_clock::now();

	auto currentTime = std::chrono::high_resolution_clock::now();
	float time = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - startTime).count();

	UniformBufferObject ubo = {};
	ubo.model = glm::rotate(glm::mat4(0.1f), time * glm::radians(20.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	ubo.view = glm::lookAt(glm::vec3(3.2f, 3.2f, 0.0f), glm::vec3(0.0f, 0.15f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	ubo.proj = glm::perspective(glm::radians(45.0f), (float)swapChain.swapChainExtent.width / (float)swapChain.swapChainExtent.height, 0.1f, 10.0f);
	ubo.proj[1][1] *= -1;

	void* data;
	vkMapMemory(device, uniformBuffer.uniformBuffersMemory[currentImage], 0, sizeof(ubo), 0, &data);
	memcpy(data, &ubo, sizeof(ubo));
	vkUnmapMemory(device, uniformBuffer.uniformBuffersMemory[currentImage]);
}

void HelloTriangleApplication::createSyncObjects()
{
	imageAvailableSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
	renderFinishedSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
	inFlightFences.resize(MAX_FRAMES_IN_FLIGHT);
	imagesInFlight.resize(swapChain.swapChainImages.size(), VK_NULL_HANDLE);

	VkSemaphoreCreateInfo semaphoreInfo = {};
	semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

	VkFenceCreateInfo fenceInfo = {};
	fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
	fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

	for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
	{
		if (vkCreateSemaphore(device, &semaphoreInfo, nullptr, &imageAvailableSemaphores[i]) != VK_SUCCESS)
		{
			throw std::runtime_error("Failed to create a VkSemaphore ('imageAvailableSemaphore')!");
		}

		if (vkCreateSemaphore(device, &semaphoreInfo, nullptr, &renderFinishedSemaphores[i]) != VK_SUCCESS)
		{
			throw std::runtime_error("Failed to create a VkSemaphore ('renderFinishedSemaphore')!");
		}

		if (vkCreateFence(device, &fenceInfo, nullptr, &inFlightFences[i]) != VK_SUCCESS)
		{
			throw std::runtime_error("Failed to create a VkFence object!");
		}
	}
}

void HelloTriangleApplication::recreateSwapChain()
{
	int width = 0, height = 0;
	glfwGetFramebufferSize(window->m_Window, &width, &height);
	while (width == 0 || height == 0)
	{
		glfwGetFramebufferSize(window->m_Window, &width, &height);
		glfwWaitEvents();
	}

	vkDeviceWaitIdle(device);

	cleanupSwapChain(uniformBuffer);

	swapChain.createSwapChain(window->m_Window, physicalDevice, device, surface);
	swapChain.createImageViews(device, imageView);
	renderPass->createRenderPass(physicalDevice, device, swapChain, image);
	graphicsPipeline->createGraphicsPipeline(device, shaderModule, swapChain, image, descriptorSetLayout, renderPass);
	image.createColorResources(device, physicalDevice, swapChain, imageView);
	image.createDepthResources(device, physicalDevice, swapChain, imageView, commandBuffer, commandPool, format, graphicsQueue);
	framebuffer.createFramebuffers(device, swapChain, image.colorImageView, image.depthImageView, renderPass->m_RenderPass);
	uniformBuffer.createUniformBuffers(physicalDevice, device, swapChain);
	descriptorPool.createDescriptorPool(device, swapChain);
	descriptorSet.createDescriptorSets(device, uniformBuffer, swapChain, descriptorSetLayout, descriptorPool, imageView, textureSampler);
	commandPool->createCommandBuffers(device, loader, renderPass->m_RenderPass, swapChain, framebuffer.swapChainFramebuffers,
		graphicsPipeline->m_Pipeline, graphicsPipeline->m_PipelineLayout->m_PipelineLayout, vertexBuffer, indexBuffer, descriptorSet);
}

void HelloTriangleApplication::mainLoop()
{
	while (!glfwWindowShouldClose(window->m_Window))
	{
		glfwPollEvents();
		drawFrame();
	}

	vkDeviceWaitIdle(device);
}

void HelloTriangleApplication::drawFrame()
{
	vkWaitForFences(device, 1, &inFlightFences[currentFrame], VK_TRUE, UINT64_MAX);

	// Acquiring an image from the swap chain
	uint32_t imageIndex;
	VkResult result = vkAcquireNextImageKHR(device, swapChain.swapChain, UINT64_MAX, imageAvailableSemaphores[currentFrame], VK_NULL_HANDLE, &imageIndex);

	if (result == VK_ERROR_OUT_OF_DATE_KHR)
	{
		recreateSwapChain();
		return;
	}
	else if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR)
	{
		throw std::runtime_error("Failed to acquire swap chain image!");
	}

	// Update uniform buffer
	updateUniformBuffer(imageIndex, uniformBuffer);

	// Check if a previous frame is using this image (i.e. there is its fence to wait on)
	if (imagesInFlight[imageIndex] != VK_NULL_HANDLE)
	{
		vkWaitForFences(device, 1, &imagesInFlight[imageIndex], VK_TRUE, UINT64_MAX);
	}

	// Mark the image as now being in use by this frame
	imagesInFlight[imageIndex] = inFlightFences[currentFrame];

	// Submitting the command buffer
	VkSubmitInfo submitInfo = {};
	submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

	VkSemaphore waitSemaphores[] = { imageAvailableSemaphores[currentFrame] };
	VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
	submitInfo.waitSemaphoreCount = 1;
	submitInfo.pWaitSemaphores = waitSemaphores;
	submitInfo.pWaitDstStageMask = waitStages;
	submitInfo.commandBufferCount = 1;
	submitInfo.pCommandBuffers = &commandPool->commandBuffers[imageIndex];

	VkSemaphore signalSemaphores[] = { renderFinishedSemaphores[currentFrame] };
	submitInfo.signalSemaphoreCount = 1;
	submitInfo.pSignalSemaphores = signalSemaphores;

	vkResetFences(device, 1, &inFlightFences[currentFrame]);

	if (vkQueueSubmit(graphicsQueue, 1, &submitInfo, inFlightFences[currentFrame]) != VK_SUCCESS)
	{
		throw std::runtime_error("Failed to submit draw command buffer!");
	}

	VkPresentInfoKHR presentInfo = {};
	presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
	presentInfo.waitSemaphoreCount = 1;
	presentInfo.pWaitSemaphores = signalSemaphores;

	VkSwapchainKHR swapChains[] = { swapChain.swapChain };
	presentInfo.swapchainCount = 1;
	presentInfo.pSwapchains = swapChains;
	presentInfo.pImageIndices = &imageIndex;
	presentInfo.pResults = nullptr; // Optional

	result = vkQueuePresentKHR(presentQueue, &presentInfo);
	if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || window->framebufferResized)
	{
		window->framebufferResized = false;
		recreateSwapChain();
	}
	else if (result != VK_SUCCESS)
	{
		throw std::runtime_error("Failed to present swap chain image!");
	}

	// vkQueueWaitIdle(presentQueue);

	currentFrame = (currentFrame + 1) % MAX_FRAMES_IN_FLIGHT;
}

void HelloTriangleApplication::cleanupSwapChain(UniformBuffer uniformBuffer)
{
	vkDestroyImageView(device, image.colorImageView, nullptr);
	vkDestroyImage(device, image.colorImage, nullptr);
	vkFreeMemory(device, image.colorImageMemory, nullptr);

	vkDestroyImageView(device, image.depthImageView, nullptr);
	vkDestroyImage(device, image.depthImage, nullptr);
	vkFreeMemory(device, image.depthImageMemory, nullptr);

	for (auto framebuffer : framebuffer.swapChainFramebuffers)
	{
		vkDestroyFramebuffer(device, framebuffer, nullptr);
	}

	vkFreeCommandBuffers(device, commandPool->commandPool, static_cast<uint32_t>(commandPool->commandBuffers.size()), commandPool->commandBuffers.data());

	graphicsPipeline->cleanUp();

	renderPass->cleanUp();

	for (auto imageView : swapChain.swapChainImageViews)
	{
		vkDestroyImageView(device, imageView, nullptr);
	}

	vkDestroySwapchainKHR(device, swapChain.swapChain, nullptr);

	for (size_t i = 0; i < swapChain.swapChainImages.size(); i++)
	{
		vkDestroyBuffer(device, uniformBuffer.uniformBuffers[i], nullptr);
		vkFreeMemory(device, uniformBuffer.uniformBuffersMemory[i], nullptr);
	}

	vkDestroyDescriptorPool(device, descriptorPool.descriptorPool, nullptr);
}

void HelloTriangleApplication::cleanup()
{
	cleanupSwapChain(uniformBuffer);

	delete textureSampler;

	vkDestroyImageView(device, imageView.m_ImageView, nullptr);
	vkDestroyImage(device, image.textureImage, nullptr);
	vkFreeMemory(device, image.textureImageMemory, nullptr);

	vkDestroyDescriptorSetLayout(device, descriptorSetLayout.descriptorSetLayout, nullptr);

	delete indexBuffer;
	delete vertexBuffer;

	for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
	{
		vkDestroySemaphore(device, renderFinishedSemaphores[i], nullptr);
		vkDestroySemaphore(device, imageAvailableSemaphores[i], nullptr);
		vkDestroyFence(device, inFlightFences[i], nullptr);
	}

	delete commandPool;

	delete graphicsPipeline;
	delete renderPass;

	vkDestroyDevice(device, nullptr);

	delete loader;
	delete surface;
	delete debug;
	delete instance;

	glfwDestroyWindow(window->m_Window);

	glfwTerminate();
}
