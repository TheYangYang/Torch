#pragma once
#include <pch/pch.h>
#include "VulkanUtils.h"

namespace core
{
	class VulkanPhysiclDevice
	{
	public:
		VulkanPhysiclDevice(VkInstance instance, VkSurfaceKHR surface);
		VkPhysicalDevice GetPhysicalDevice() const { return m_PhysicalDevice; }

	private:
		VkPhysicalDevice m_PhysicalDevice = VK_NULL_HANDLE;
	};
}