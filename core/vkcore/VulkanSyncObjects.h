#pragma once
#include <pch/pch.h>

namespace core
{
	class VulkanSyncObjects
	{
	public:
		VulkanSyncObjects(VkDevice logicDevice);

		VkSemaphore& GetImageAvailableSemaphoreRef() { return m_ImageAvailableSemaphore; }
		VkSemaphore& GetRenderFinishedSemaphoreRef() { return m_RenderFinishedSemaphore; }
		VkFence& GetInFlightFenceRef() { return m_InFlightFence; }
	private:
		VkSemaphore m_ImageAvailableSemaphore;
		VkSemaphore m_RenderFinishedSemaphore;
		VkFence m_InFlightFence;
	};
}