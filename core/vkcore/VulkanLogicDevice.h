#pragma once
#include <pch/pch.h>

namespace core
{
    class VulkanLogicDevice
    {
    public:
        VulkanLogicDevice(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface);
        ~VulkanLogicDevice();

        VkDevice GetLogicDevice() const { return m_LogicDevice; }
        VkQueue GetGraphicsQueue() const { return m_GraphicsQueue; }
        VkQueue GetPresentQueue() const { return m_PresentQueue; }

        VkPipelineCache GetPipelineCache() const { return m_PipelineCache; }
        VkDescriptorPool GetDescriptorPool() const { return m_DescriptorPool; }

        void CleanLogicDevice()
        {
            vkDestroyDevice(m_LogicDevice, nullptr);
        }


    private:
        void CreatePipelineCache();
        void CreateDescriptorPool();

    private:
        VkQueue m_GraphicsQueue = VK_NULL_HANDLE;
        VkQueue m_PresentQueue = VK_NULL_HANDLE;
        VkDevice m_LogicDevice = VK_NULL_HANDLE;

        VkPipelineCache m_PipelineCache = VK_NULL_HANDLE;
        VkDescriptorPool m_DescriptorPool = VK_NULL_HANDLE;
    };
}
