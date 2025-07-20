#pragma once
#include <pch/pch.h>
#include "VulkanUtils.h"

namespace core
{
	
	class VulkanSwapChain
	{
	public:
		VulkanSwapChain(VkPhysicalDevice physicalDevice, VkDevice logicDevice, VkSurfaceKHR surface, GLFWwindow* window);

		VkSwapchainKHR GetSwapChain() const { return m_SwapChain; }

		std::vector<VkImage>& GetSwapChainImagesRef() { return m_SwapChainImages; }
		VkFormat& GetSwapChainImageFormatRef() { return m_SwapChainImageFormat; }
		VkExtent2D& GetSwapChainExtentRef() { return m_SwapChainExtent; }
		std::vector<VkImageView>& GetSwapChainImageViewsRef() { return m_SwapChainImageViews; }

	private:
		VkSwapchainKHR m_SwapChain;


		std::vector<VkImage> m_SwapChainImages;
		VkFormat m_SwapChainImageFormat;
		VkExtent2D m_SwapChainExtent;
		std::vector<VkImageView> m_SwapChainImageViews;
	};
}