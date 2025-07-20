#pragma once
#include <pch/pch.h>

namespace core
{
	class VulkanCommandPool
	{
	public:
		VulkanCommandPool(VkPhysicalDevice physicalDevice, VkDevice logicDevice, VkSurfaceKHR surface);
		VkCommandPool GetCommandPool() const { return m_CommandPool; }
	private:
		VkCommandPool m_CommandPool;
	};
}