#include "UniformBuffer.h"

#include "SwapChain.h"
#include "Buffer.h"


void UniformBuffer::createUniformBuffers(VkDevice device, VkPhysicalDevice hPhysicalDevice, SwapChain swapChain, Buffer buffer)
{
	VkDeviceSize bufferSize = sizeof(UniformBufferObject);

	uniformBuffers.resize(swapChain.swapChainImages.size());
	uniformBuffersMemory.resize(swapChain.swapChainImages.size());

	for (size_t i = 0; i < swapChain.swapChainImages.size(); i++)
	{
		buffer.createBuffer(device, hPhysicalDevice, bufferSize, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
			VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
			uniformBuffers[i], uniformBuffersMemory[i]);
	}
}
