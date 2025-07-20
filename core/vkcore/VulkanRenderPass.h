#pragma once
#include <pch/pch.h>
#include "VulkanSwapChain.h"

namespace core
{
	class VulkanRenderPass
	{
	public:
		VulkanRenderPass(VulkanSwapChain& swapChain, VkDevice logicDevice);

		VkRenderPass GetRenderPass() const { return m_RenderPass; }
	private:

		VkRenderPass m_RenderPass;
	};
}