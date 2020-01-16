#include "CommandPool.h"

#include <stdexcept>

void CommandPool::createCommandPool(PhysicalDevice physicalDevice, VkPhysicalDevice hPhysicalDevice, VkDevice device, VkSurfaceKHR surfaceKHR)
{
	QueueFamilyIndices queueFamilyIndices = physicalDevice.findQueueFamilies(hPhysicalDevice, surfaceKHR);

	VkCommandPoolCreateInfo poolInfo = {};
	poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
	poolInfo.queueFamilyIndex = queueFamilyIndices.graphicsFamily.value();

	if (vkCreateCommandPool(device, &poolInfo, nullptr, &commandPool) != VK_SUCCESS)
	{
		throw std::runtime_error("Failed to create Command pool!");
	}
}
