#include "VulkanCommandBuffer.h"

namespace core
{
	VulkanCommandBuffer::VulkanCommandBuffer(VkCommandPool commandPool, VkDevice logicDevice)
	{
        m_CommandBuffers.resize(MAX_FRAMES_IN_FLIGHT);

        VkCommandBufferAllocateInfo allocInfo{};
        allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        allocInfo.commandPool = commandPool;
        allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
        allocInfo.commandBufferCount = (uint32_t)m_CommandBuffers.size();

        if (vkAllocateCommandBuffers(logicDevice, &allocInfo, m_CommandBuffers.data()) != VK_SUCCESS) 
        {
            throw std::runtime_error("failed to allocate command buffers!");
        }
	}
}