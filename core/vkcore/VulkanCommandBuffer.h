#pragma once
#include <pch/pch.h>

namespace core
{
	class VulkanCommandBuffer
	{
	public:
		VulkanCommandBuffer(VkCommandPool commandPool, VkDevice logicDevice);
		VkCommandBuffer& GetCommandBufferRef() { return m_CommandBuffer; }
	private:
		VkCommandBuffer m_CommandBuffer;
	};
}