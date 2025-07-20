#pragma once
#include <pch/pch.h>
#include <core/vkcore/VulkanInstance.h>
#include <core/vkcore/VulkanUtils.h>
#include <core/vkcore/VulkanSurface.h>
#include <core/vkcore/VulkanPhysicalDevice.h>
#include <core/vkcore/VulkanLogicDevice.h>
#include <core/vkcore/VulkanSwapChain.h>
#include <core/vkcore/VulkanImageView.h>
#include <core/vkcore/VulkanGraphicsPipeline.h>
#include <core/vkcore/VulkanRenderPass.h>
#include <core/vkcore/VulkanFramebuffer.h>
#include <core/vkcore/VulkanCommandPool.h>
#include <core/vkcore/VulkanCommandBuffer.h>
#include <core/vkcore/VulkanSyncObjects.h>

namespace core
{

	class TorchVulkanContext 
	{
	public:
		static TorchVulkanContext& Get(GLFWwindow* window)
		{
			static TorchVulkanContext instance(window);
			return instance;
		}

		TorchVulkanContext(GLFWwindow* window);
		~TorchVulkanContext();
		void DrawFrame();
		void OnUpdate();


		void TorchVulkanContext::ReCreate();
		
	private:
		static GLFWwindow* s_WindowPtr;

		void RecordCommandBuffer(uint32_t imageIndex);

		//vk core
		Scope<VulkanInstance> m_Instance;
		Scope<VulkanSurface> m_Surface;
		Scope<VulkanPhysiclDevice> m_PhysicalDevice;
		Scope<VulkanLogicDevice> m_LogicDevice;
		Scope<VulkanSwapChain> m_SwapChain;
		Scope<VulkanImageView> m_ImageView;
		Scope<VulkanGraphicsPipeline> m_GraphicsPipeline;
		Scope<VulkanRenderPass> m_RenderPass;
		Scope<VulkanFramebuffer> m_Framebuffer;
		Scope<VulkanCommandPool> m_CommandPool;
		Scope<VulkanCommandBuffer> m_CommandBuffer;
		Scope<VulkanSyncObjects> m_SyncObjects;
	};
}
