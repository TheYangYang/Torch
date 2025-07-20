#pragma once
#include <pch/pch.h>

namespace core
{
	class VulkanCommandPool
	{
	public:
		VulkanCommandPool(VkPhysicalDevice physicalDevice, VkDevice logicDevice, VkSurfaceKHR surface);
		VkCommandPool GetCommandPool() const { return m_CommandPool; }

		void CleanCommandPool(VkDevice logicDevice)
		{
			vkDestroyCommandPool(logicDevice, m_CommandPool, nullptr);
		}
	private:
		VkCommandPool m_CommandPool;
	};
}