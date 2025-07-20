#include "TorchVulkanContext.h"
#include "core/platform/Window.h"
#include <imgui.h>
#include <backends/imgui_impl_vulkan.h>
#include <backends/imgui_impl_glfw.h>
#include <utils/ServiceLocator.h>

namespace core
{
	GLFWwindow* TorchVulkanContext::s_WindowPtr = nullptr;
	TorchVulkanContext::TorchVulkanContext(GLFWwindow* window)
	{
		TORCH_LOG_INFO("Initialize the Vulkan.");

		s_WindowPtr = window;
		m_Instance = CreateScope<VulkanInstance>();
		VulkanUtils::SetupDebugMessenger(m_Instance->GetInstance());
		m_Surface = CreateScope<VulkanSurface>(m_Instance->GetInstance(), s_WindowPtr);
		m_PhysicalDevice = CreateScope<VulkanPhysiclDevice>(m_Instance->GetInstance(), m_Surface->GetSurface());
		m_LogicDevice = CreateScope<VulkanLogicDevice>(m_PhysicalDevice->GetPhysicalDevice(), m_Surface->GetSurface());
		m_SwapChain = CreateScope<VulkanSwapChain>(m_PhysicalDevice->GetPhysicalDevice(), m_LogicDevice->GetLogicDevice(), m_Surface->GetSurface(), s_WindowPtr);
		m_ImageView = CreateScope<VulkanImageView>(*m_SwapChain, m_LogicDevice->GetLogicDevice());
		m_RenderPass = CreateScope<VulkanRenderPass>(*m_SwapChain, m_LogicDevice->GetLogicDevice());
		m_GraphicsPipeline = CreateScope<VulkanGraphicsPipeline>(m_LogicDevice->GetLogicDevice(), m_RenderPass->GetRenderPass());
		m_Framebuffer = CreateScope<VulkanFramebuffer>(*m_SwapChain, m_RenderPass->GetRenderPass(), m_LogicDevice->GetLogicDevice());
		m_CommandPool = CreateScope<VulkanCommandPool>(m_PhysicalDevice->GetPhysicalDevice(), m_LogicDevice->GetLogicDevice(), m_Surface->GetSurface());
		m_CommandBuffer = CreateScope<VulkanCommandBuffer>(m_CommandPool->GetCommandPool(), m_LogicDevice->GetLogicDevice());
        m_SyncObjects = CreateScope<VulkanSyncObjects>(m_LogicDevice->GetLogicDevice());

		TORCH_LOG_INFO("Finished initialization of Vulkan.");
	}
	void TorchVulkanContext::ReCreate()
	{

		TORCH_LOG_INFO("recreate");
	}

	void TorchVulkanContext::DrawFrame()
	{  
        vkWaitForFences(m_LogicDevice->GetLogicDevice(), 1, &m_SyncObjects->GetInFlightFenceRef(), VK_TRUE, UINT64_MAX);
        vkResetFences(m_LogicDevice->GetLogicDevice(), 1, &m_SyncObjects->GetInFlightFenceRef());

        uint32_t imageIndex;
        vkAcquireNextImageKHR(m_LogicDevice->GetLogicDevice(), m_SwapChain->GetSwapChain(), UINT64_MAX, m_SyncObjects->GetImageAvailableSemaphoreRef(), VK_NULL_HANDLE, &imageIndex);

        vkResetCommandBuffer(m_CommandBuffer->GetCommandBufferRef(), /*VkCommandBufferResetFlagBits*/ 0);
        RecordCommandBuffer(imageIndex);

        VkSubmitInfo submitInfo{};
        submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

        VkSemaphore waitSemaphores[] = { m_SyncObjects->GetImageAvailableSemaphoreRef()};
        VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
        submitInfo.waitSemaphoreCount = 1;
        submitInfo.pWaitSemaphores = waitSemaphores;
        submitInfo.pWaitDstStageMask = waitStages;

        submitInfo.commandBufferCount = 1;
        submitInfo.pCommandBuffers = &m_CommandBuffer->GetCommandBufferRef();

        VkSemaphore signalSemaphores[] = { m_SyncObjects->GetRenderFinishedSemaphoreRef()};
        submitInfo.signalSemaphoreCount = 1;
        submitInfo.pSignalSemaphores = signalSemaphores;

        if (vkQueueSubmit(m_LogicDevice->GetGraphicsQueue(), 1, &submitInfo, m_SyncObjects->GetInFlightFenceRef()) != VK_SUCCESS) {
            throw std::runtime_error("failed to submit draw command buffer!");
        }

        VkPresentInfoKHR presentInfo{};
        presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;

        presentInfo.waitSemaphoreCount = 1;
        presentInfo.pWaitSemaphores = signalSemaphores;

        VkSwapchainKHR swapChains[] = { m_SwapChain->GetSwapChain()};
        presentInfo.swapchainCount = 1;
        presentInfo.pSwapchains = swapChains;

        presentInfo.pImageIndices = &imageIndex;

        vkQueuePresentKHR(m_LogicDevice->GetPresentQueue(), &presentInfo);
	}


    void TorchVulkanContext::RecordCommandBuffer(uint32_t imageIndex) {
        VkCommandBufferBeginInfo beginInfo{};
        beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

        if (vkBeginCommandBuffer(m_CommandBuffer->GetCommandBufferRef(), &beginInfo) != VK_SUCCESS) {
            throw std::runtime_error("failed to begin recording command buffer!");
        }

        VkRenderPassBeginInfo renderPassInfo{};
        renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
        renderPassInfo.renderPass = m_RenderPass->GetRenderPass();
        renderPassInfo.framebuffer = m_Framebuffer->GetSwapChainFramebuffersRef()[imageIndex];
        renderPassInfo.renderArea.offset = { 0, 0 };
        renderPassInfo.renderArea.extent = m_SwapChain->GetSwapChainExtentRef();

        VkClearValue clearColor = { {{0.0f, 0.0f, 0.0f, 1.0f}} };
        renderPassInfo.clearValueCount = 1;
        renderPassInfo.pClearValues = &clearColor;

        vkCmdBeginRenderPass(m_CommandBuffer->GetCommandBufferRef(), &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

        vkCmdBindPipeline(m_CommandBuffer->GetCommandBufferRef(), VK_PIPELINE_BIND_POINT_GRAPHICS, m_GraphicsPipeline->GetGraphicsPipeline());

        VkViewport viewport{};
        viewport.x = 0.0f;
        viewport.y = 0.0f;
        viewport.width = (float)m_SwapChain->GetSwapChainExtentRef().width;
        viewport.height = (float)m_SwapChain->GetSwapChainExtentRef().height;
        viewport.minDepth = 0.0f;
        viewport.maxDepth = 1.0f;
        vkCmdSetViewport(m_CommandBuffer->GetCommandBufferRef(), 0, 1, &viewport);

        VkRect2D scissor{};
        scissor.offset = { 0, 0 };
        scissor.extent = m_SwapChain->GetSwapChainExtentRef();
        vkCmdSetScissor(m_CommandBuffer->GetCommandBufferRef(), 0, 1, &scissor);

        vkCmdDraw(m_CommandBuffer->GetCommandBufferRef(), 3, 1, 0, 0);

        vkCmdEndRenderPass(m_CommandBuffer->GetCommandBufferRef());

        if (vkEndCommandBuffer(m_CommandBuffer->GetCommandBufferRef()) != VK_SUCCESS) {
            throw std::runtime_error("failed to record command buffer!");
        }
    }

	TorchVulkanContext::~TorchVulkanContext()
	{

	}
	

    void TorchVulkanContext::OnUpdate()
    {
        TORCH_LOG_INFO("on update");
    }

}