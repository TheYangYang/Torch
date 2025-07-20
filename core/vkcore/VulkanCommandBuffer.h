#pragma once
#include <pch/pch.h>

namespace core
{
	class VulkanCommandBuffer
	{
	public:
		VulkanCommandBuffer(VkCommandPool commandPool, VkDevice logicDevice);
		std::vector<VkCommandBuffer>& GetCommandBuffersRef() { return m_CommandBuffers; }
	private:
		std::vector<VkCommandBuffer> m_CommandBuffers;
	};
}