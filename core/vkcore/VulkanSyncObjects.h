#pragma once
#include <pch/pch.h>

namespace core
{
	class VulkanSyncObjects
	{
	public:
		VulkanSyncObjects(VkDevice logicDevice, uint32_t swapchainImageCount);

		std::vector<VkSemaphore>& GetImageAvailableSemaphoresRef() { return m_ImageAvailableSemaphores; }
		std::vector<VkSemaphore>& GetRenderFinishedSemaphoresRef() { return m_RenderFinishedSemaphores; }
		std::vector<VkFence>& GetInFlightFencesRef() { return m_InFlightFences; }

		void CleanSyncObjects(VkDevice logicDevice)
		{
			for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
			{
				vkDestroySemaphore(logicDevice, m_RenderFinishedSemaphores[i], nullptr);
				vkDestroySemaphore(logicDevice, m_ImageAvailableSemaphores[i], nullptr);
				vkDestroyFence(logicDevice, m_InFlightFences[i], nullptr);
			}
		}
	private:
		std::vector<VkSemaphore> m_ImageAvailableSemaphores;
		std::vector<VkSemaphore>m_RenderFinishedSemaphores;
		std::vector<VkFence> m_InFlightFences;
		uint32_t m_SwapchainImageCount; 
	};
}