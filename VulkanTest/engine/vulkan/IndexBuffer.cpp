#include "IndexBuffer.h"

#include "../Loader.h"
#include "Buffer.h"
#include "CommandBuffer.h"
#include "CommandPool.h"



void IndexBuffer::copyBuffer(VkDevice device, VkQueue graphicsQueue, CommandBuffer commandBuffer, CommandPool commandPool,
	VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size)
{
	VkCommandBuffer cmdBuffer = commandBuffer.beginSingleTimeCommands(device, commandPool.commandPool);

	VkBufferCopy copyRegion = {};
	copyRegion.size = size;
	vkCmdCopyBuffer(cmdBuffer, srcBuffer, dstBuffer, 1, &copyRegion);

	commandBuffer.endSingleTimeCommands(device, cmdBuffer, graphicsQueue, commandPool.commandPool);
}

void IndexBuffer::createIndexBuffer(VkPhysicalDevice hPhysicalDevice, VkDevice device, Loader loader, Buffer buffer,
	VkQueue graphicsQueue, CommandBuffer commandBuffer, CommandPool commandPool)
{
	VkDeviceSize bufferSize = sizeof(loader.indices[0]) * loader.indices.size();

	VkBuffer stagingBuffer;
	VkDeviceMemory stagingBufferMemory;
	buffer.createBuffer(device, hPhysicalDevice, bufferSize,
		VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
		VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
		stagingBuffer, stagingBufferMemory);

	void* data;
	vkMapMemory(device, stagingBufferMemory, 0, bufferSize, 0, &data);
	memcpy(data, loader.indices.data(), (size_t)bufferSize);
	vkUnmapMemory(device, stagingBufferMemory);

	buffer.createBuffer(device, hPhysicalDevice, bufferSize,
		VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT,
		VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
		indexBuffer, indexBufferMemory);

	copyBuffer(device, graphicsQueue, commandBuffer, commandPool, stagingBuffer, indexBuffer, bufferSize);

	vkDestroyBuffer(device, stagingBuffer, nullptr);
	vkFreeMemory(device, stagingBufferMemory, nullptr);
}
