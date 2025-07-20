#pragma once
#include <pch/pch.h>
#include "VulkanSwapChain.h"

namespace core
{
	class VulkanImageView
	{
	public:
		VulkanImageView(VulkanSwapChain& swapChain, VkDevice logicDevice);

		void CreateImageViews(VulkanSwapChain& swapChain, VkDevice logicDevice);
	private:
	};
}