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
	createGraphicsPipeline();
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
	commandPool->createCommandBuffers(device, loader, renderPass->m_RenderPass, swapChain, framebuffer.swapChainFramebuffers, graphicsPipeline, pipelineLayout->pipelineLayout,
		vertexBuffer, indexBuffer, descriptorSet);
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
	createGraphicsPipeline();
	image.createColorResources(device, physicalDevice, swapChain, imageView);
	image.createDepthResources(device, physicalDevice, swapChain, imageView, commandBuffer, commandPool, format, graphicsQueue);
	framebuffer.createFramebuffers(device, swapChain, image.colorImageView, image.depthImageView, renderPass->m_RenderPass);
	uniformBuffer.createUniformBuffers(physicalDevice, device, swapChain);
	descriptorPool.createDescriptorPool(device, swapChain);
	descriptorSet.createDescriptorSets(device, uniformBuffer, swapChain, descriptorSetLayout, descriptorPool, imageView, textureSampler);
	commandPool->createCommandBuffers(device, loader, renderPass->m_RenderPass, swapChain, framebuffer.swapChainFramebuffers, graphicsPipeline, pipelineLayout->pipelineLayout,
		vertexBuffer, indexBuffer, descriptorSet);
}

void HelloTriangleApplication::createGraphicsPipeline()
{
	auto vertShaderCode = Loader::readFile("shaders/shader_vertex.spv");
	auto fragShaderCode = Loader::readFile("shaders/shader_fragment.spv");

	VkShaderModule vertShaderModule = shaderModule.createShaderModule(device, vertShaderCode);
	VkShaderModule fragShaderModule = shaderModule.createShaderModule(device, fragShaderCode);

	std::cout << std::endl;
	std::cout << "Loading vertex and fragment shaders in binary SPIR-V format:" << std::endl;
	std::cout << "\t" << "Vertex shader size:" << vertShaderCode.size() << std::endl;
	std::cout << "\t" << "Fragment shader size:" << fragShaderCode.size() << std::endl;

	VkPipelineShaderStageCreateInfo vertShaderStageInfo = {};
	vertShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	vertShaderStageInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
	vertShaderStageInfo.module = vertShaderModule;
	vertShaderStageInfo.pName = "main";

	VkPipelineShaderStageCreateInfo fragShaderStageInfo = {};
	fragShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	fragShaderStageInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
	fragShaderStageInfo.module = fragShaderModule;
	fragShaderStageInfo.pName = "main";

	VkPipelineShaderStageCreateInfo shaderStages[] = { vertShaderStageInfo, fragShaderStageInfo };

	// Fixed functions - Vertex input
	VkPipelineVertexInputStateCreateInfo vertexInputInfo = {};
	vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;

	auto bindingDescription = Vertex::getBindingDescription();
	auto attributeDescriptions = Vertex::getAttributeDescriptions();

	vertexInputInfo.vertexBindingDescriptionCount = 1;
	vertexInputInfo.vertexAttributeDescriptionCount = static_cast<uint32_t>(attributeDescriptions.size());
	vertexInputInfo.pVertexBindingDescriptions = &bindingDescription; // Optional
	vertexInputInfo.pVertexAttributeDescriptions = attributeDescriptions.data(); // Optional

	// Fixed functions - Input assembly
	VkPipelineInputAssemblyStateCreateInfo inputAssembly = {};
	inputAssembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
	inputAssembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
	inputAssembly.primitiveRestartEnable = VK_FALSE;

	// Fixed functions - Viewports and scissors
	VkViewport viewport = {};
	viewport.x = 0.0f;
	viewport.y = 0.0f;
	viewport.width = (float)swapChain.swapChainExtent.width;
	viewport.height = (float)swapChain.swapChainExtent.height;
	viewport.minDepth = 0.0f;
	viewport.maxDepth = 1.0f;

	VkRect2D scissor = {};
	scissor.offset = { 0, 0 };
	scissor.extent = swapChain.swapChainExtent;

	VkPipelineViewportStateCreateInfo viewportState = {};
	viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
	viewportState.viewportCount = 1;
	viewportState.pViewports = &viewport;
	viewportState.scissorCount = 1;
	viewportState.pScissors = &scissor;

	// Fixed functions - Rasterizer
	VkPipelineRasterizationStateCreateInfo rasterizer = {};
	rasterizer.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
	rasterizer.depthClampEnable = VK_FALSE;
	rasterizer.rasterizerDiscardEnable = VK_FALSE;
	rasterizer.polygonMode = VK_POLYGON_MODE_FILL;
	rasterizer.lineWidth = 1.0f;
	rasterizer.cullMode = VK_CULL_MODE_BACK_BIT;
	rasterizer.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE;
	rasterizer.depthBiasEnable = VK_FALSE;

	// Fixed functions - Multisampling
	VkPipelineMultisampleStateCreateInfo multisampling = {};
	multisampling.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
	multisampling.sampleShadingEnable = VK_TRUE; // enable sample shading in the pipeline
	multisampling.minSampleShading = 0.2f; // min fraction for sample shading; closer to one is smoother
	multisampling.rasterizationSamples = image.msaaSamples;

	VkPipelineDepthStencilStateCreateInfo depthStencil = {};
	depthStencil.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
	depthStencil.depthTestEnable = VK_TRUE;
	depthStencil.depthWriteEnable = VK_TRUE;
	depthStencil.depthCompareOp = VK_COMPARE_OP_LESS;
	depthStencil.depthBoundsTestEnable = VK_FALSE;
	depthStencil.stencilTestEnable = VK_FALSE;

	// Fixed functions - Color blending
	VkPipelineColorBlendAttachmentState colorBlendAttachment = {};
	colorBlendAttachment.colorWriteMask =
		VK_COLOR_COMPONENT_R_BIT |
		VK_COLOR_COMPONENT_G_BIT |
		VK_COLOR_COMPONENT_B_BIT |
		VK_COLOR_COMPONENT_A_BIT;
	colorBlendAttachment.blendEnable = VK_FALSE;

	VkPipelineColorBlendStateCreateInfo colorBlending = {};
	colorBlending.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
	colorBlending.logicOpEnable = VK_FALSE;
	colorBlending.logicOp = VK_LOGIC_OP_COPY; // Optional
	colorBlending.attachmentCount = 1;
	colorBlending.pAttachments = &colorBlendAttachment;
	colorBlending.blendConstants[0] = 0.0f; // Optional
	colorBlending.blendConstants[1] = 0.0f; // Optional
	colorBlending.blendConstants[2] = 0.0f; // Optional
	colorBlending.blendConstants[3] = 0.0f; // Optional

	// Fixed functions - Pipeline layout
	pipelineLayout = new PipelineLayout(device, descriptorSetLayout);

	VkGraphicsPipelineCreateInfo pipelineInfo = {};
	pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
	// shader-stage
	pipelineInfo.stageCount = 2;
	pipelineInfo.pStages = shaderStages;
	// fixed-function stage
	pipelineInfo.pVertexInputState = &vertexInputInfo;
	pipelineInfo.pInputAssemblyState = &inputAssembly;
	pipelineInfo.pViewportState = &viewportState;
	pipelineInfo.pRasterizationState = &rasterizer;
	pipelineInfo.pMultisampleState = &multisampling;
	pipelineInfo.pDepthStencilState = &depthStencil;
	pipelineInfo.pColorBlendState = &colorBlending;
	// pipeline layout
	pipelineInfo.layout = pipelineLayout->pipelineLayout;
	// render pass
	pipelineInfo.renderPass = renderPass->m_RenderPass;
	pipelineInfo.subpass = 0;

	pipelineInfo.basePipelineHandle = VK_NULL_HANDLE; // Optional

	if (vkCreateGraphicsPipelines(device, VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &graphicsPipeline) != VK_SUCCESS)
	{
		throw std::runtime_error("Failed to create the Graphics pipeline!");
	}

	vkDestroyShaderModule(device, fragShaderModule, nullptr);
	vkDestroyShaderModule(device, vertShaderModule, nullptr);
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

	vkDestroyPipeline(device, graphicsPipeline, nullptr);
	
	delete pipelineLayout;

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

	delete renderPass;

	vkDestroyDevice(device, nullptr);

	delete loader;
	delete surface;
	delete debug;
	delete instance;

	glfwDestroyWindow(window->m_Window);

	glfwTerminate();
}
