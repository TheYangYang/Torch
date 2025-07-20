#include "VulkanCommandBuffer.h"

namespace core
{
	VulkanCommandBuffer::VulkanCommandBuffer(VkCommandPool commandPool, VkDevice logicDevice)
	{
        VkCommandBufferAllocateInfo allocInfo{};
        allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        allocInfo.commandPool = commandPool;
        allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
        allocInfo.commandBufferCount = 1;

        if (vkAllocateCommandBuffers(logicDevice, &allocInfo, &m_CommandBuffer) != VK_SUCCESS) 
        {
            throw std::runtime_error("failed to allocate command buffers!");
        }
	}
}