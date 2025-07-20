#include "VulkanFramebuffer.h"

namespace core
{
	VulkanFramebuffer::VulkanFramebuffer(VulkanSwapChain& swapChain, VkRenderPass renderPass, VkDevice logicDevice)
	{
        CreateFramebuffers(swapChain, renderPass, logicDevice);
	}
    void VulkanFramebuffer::CreateFramebuffers(VulkanSwapChain& swapChain, VkRenderPass renderPass, VkDevice logicDevice)
    {
        auto& swapChainImageViews = swapChain.GetSwapChainImageViewsRef();
        auto& swapChainExtent = swapChain.GetSwapChainExtentRef();

        m_SwapChainFramebuffers.resize(swapChainImageViews.size());

        for (size_t i = 0; i < swapChainImageViews.size(); i++) {
            VkImageView attachments[] = {
                swapChainImageViews[i]
            };

            VkFramebufferCreateInfo framebufferInfo{};
            framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
            framebufferInfo.renderPass = renderPass;
            framebufferInfo.attachmentCount = 1;
            framebufferInfo.pAttachments = attachments;
            framebufferInfo.width = swapChainExtent.width;
            framebufferInfo.height = swapChainExtent.height;
            framebufferInfo.layers = 1;

            if (vkCreateFramebuffer(logicDevice, &framebufferInfo, nullptr, &m_SwapChainFramebuffers[i]) != VK_SUCCESS) {
                throw std::runtime_error("failed to create framebuffer!");
            }
        }
    }
}