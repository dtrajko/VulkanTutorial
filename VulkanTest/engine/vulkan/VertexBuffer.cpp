#include "VertexBuffer.h"

#include "../Loader.h"
#include "IndexBuffer.h"
#include "CommandBuffer.h"
#include "CommandPool.h"
#include "Buffer.h"


void VertexBuffer::createVertexBuffer(Loader loader, VkDevice device, VkPhysicalDevice hPhysicalDevice,
	IndexBuffer indexBuffer, VkQueue graphicsQueue, CommandBuffer commandBuffer, CommandPool commandPool, Buffer buffer)
{
	VkDeviceSize bufferSize = sizeof(loader.vertices[0]) * loader.vertices.size();

	VkBuffer stagingBuffer;
	VkDeviceMemory stagingBufferMemory;
	buffer.createBuffer(device, hPhysicalDevice, bufferSize,
		VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
		VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
		stagingBuffer, stagingBufferMemory);

	void* data;
	vkMapMemory(device, stagingBufferMemory, 0, bufferSize, 0, &data);
	memcpy(data, loader.vertices.data(), (size_t)bufferSize);
	vkUnmapMemory(device, stagingBufferMemory);

	buffer.createBuffer(device, hPhysicalDevice, bufferSize,
		VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
		VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
		vertexBuffer, vertexBufferMemory);

	indexBuffer.copyBuffer(device, graphicsQueue, commandBuffer, commandPool, stagingBuffer, vertexBuffer, bufferSize);

	vkDestroyBuffer(device, stagingBuffer, nullptr);
	vkFreeMemory(device, stagingBufferMemory, nullptr);
}
