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
	private:
		std::vector<VkFramebuffer> m_SwapChainFramebuffers;
	};
}