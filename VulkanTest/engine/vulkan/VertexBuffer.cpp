#include "VertexBuffer.h"

#include "../Loader.h"
#include "IndexBuffer.h"
#include "CommandBuffer.h"
#include "CommandPool.h"
#include "Buffer.h"


VertexBuffer::VertexBuffer(VkDevice device, VkPhysicalDevice hPhysicalDevice, Loader loader, IndexBuffer* indexBuffer,
	VkQueue graphicsQueue, CommandBuffer commandBuffer, CommandPool* commandPool) : m_device(device)
{
	VkDeviceSize bufferSize = sizeof(loader.vertices[0]) * loader.vertices.size();

	VkBuffer stagingBuffer;
	VkDeviceMemory stagingBufferMemory;

	Buffer* oStagingBuffer = new Buffer(device, hPhysicalDevice, bufferSize,
		VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
		VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

	stagingBuffer = oStagingBuffer->m_Buffer;
	stagingBufferMemory = oStagingBuffer->m_Memory;

	void* data;
	vkMapMemory(device, stagingBufferMemory, 0, bufferSize, 0, &data);
	memcpy(data, loader.vertices.data(), (size_t)bufferSize);
	vkUnmapMemory(device, stagingBufferMemory);
	
	Buffer* oVertexBuffer = new Buffer(device, hPhysicalDevice, bufferSize,
		VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
		VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

	m_Buffer = oVertexBuffer->m_Buffer;
	m_Memory = oVertexBuffer->m_Memory;

	indexBuffer->copyBuffer(device, graphicsQueue, commandBuffer, commandPool, stagingBuffer, m_Buffer, bufferSize);

	vkDestroyBuffer(device, stagingBuffer, nullptr);
	vkFreeMemory(device, stagingBufferMemory, nullptr);

	delete oVertexBuffer;
	delete oStagingBuffer;
}

VertexBuffer::~VertexBuffer()
{
	vkDestroyBuffer(m_device, m_Buffer, nullptr);
	vkFreeMemory(m_device, m_Memory, nullptr);
}
