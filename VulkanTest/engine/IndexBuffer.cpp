#include "IndexBuffer.h"

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
