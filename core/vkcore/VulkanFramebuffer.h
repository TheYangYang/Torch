#pragma once
#include <pch/pch.h>
#include "VulkanSwapChain.h"

namespace core
{
	class VulkanFramebuffer
	{
	public:
		VulkanFramebuffer(VulkanSwapChain& swapChain, VkRenderPass renderPass, VkDevice logicDevice);
		std::vector<VkFramebuffer>& GetSwapChainFramebuffersRef() { return m_SwapChainFramebuffers; }
		
		void CleanupSwapChain(VkDevice logicDevice)
		{
			for (auto framebuffer : m_SwapChainFramebuffers) {
				vkDestroyFramebuffer(logicDevice, framebuffer, nullptr);
			}
		}

		void CreateFramebuffers(VulkanSwapChain& swapChain, VkRenderPass renderPass, VkDevice logicDevice);
	
	private:
		std::vector<VkFramebuffer> m_SwapChainFramebuffers;
	};
}