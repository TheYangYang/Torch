#include "VulkanSyncObjects.h"
namespace core
{
    VulkanSyncObjects::VulkanSyncObjects(VkDevice logicDevice, uint32_t swapchainImageCount)
        :m_SwapchainImageCount(swapchainImageCount)
    {
        // CHANGE THESE LINES - use swapchainImageCount instead of MAX_FRAMES_IN_FLIGHT
        m_ImageAvailableSemaphores.resize(swapchainImageCount); 
        m_RenderFinishedSemaphores.resize(swapchainImageCount); 
        m_InFlightFences.resize(MAX_FRAMES_IN_FLIGHT);           

        VkSemaphoreCreateInfo semaphoreInfo{};
        semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
        VkFenceCreateInfo fenceInfo{};
        fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
        fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

        // CHANGE THIS LOOP - create semaphores for each swapchain image
        for (size_t i = 0; i < swapchainImageCount; i++) { 
            if (vkCreateSemaphore(logicDevice, &semaphoreInfo, nullptr, &m_ImageAvailableSemaphores[i]) != VK_SUCCESS ||
                vkCreateSemaphore(logicDevice, &semaphoreInfo, nullptr, &m_RenderFinishedSemaphores[i]) != VK_SUCCESS) {
                throw std::runtime_error("failed to create semaphores!");
            }
        }

        // ADD THIS LOOP - create fences for frames in flight
        for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
            if (vkCreateFence(logicDevice, &fenceInfo, nullptr, &m_InFlightFences[i]) != VK_SUCCESS) {
                throw std::runtime_error("failed to create fence!");
            }
        }
    }
}