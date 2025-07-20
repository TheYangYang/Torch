#pragma once
#include <pch/pch.h>

namespace core
{
	class VulkanLogicDevice
	{
	public:
		VulkanLogicDevice(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface);
		VkDevice GetLogicDevice() const { return m_LogicDevice; }
		VkQueue GetGraphicsQueue() const { return m_GraphicsQueue; }
		VkQueue GetPresentQueue() const { return m_PresentQueue; }
	private:
		VkQueue m_GraphicsQueue;
		VkQueue m_PresentQueue;
		VkDevice m_LogicDevice;
	};
}