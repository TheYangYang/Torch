#pragma once
#include <pch/pch.h>

namespace core
{
	class VulkanGraphicsPipeline
	{
	public:
		VulkanGraphicsPipeline(VkDevice logicDevice, VkRenderPass renderPass);
		VkPipeline GetGraphicsPipeline() const { return m_GraphicsPipeline; }

		void CleanPipelineAndLayout(VkDevice logicDevice)
		{
			vkDestroyPipeline(logicDevice, m_GraphicsPipeline, nullptr);
			vkDestroyPipelineLayout(logicDevice, m_PipelineLayout, nullptr);
		}
	private:
		VkShaderModule CreateShaderModule(const std::vector<char>& code, VkDevice logicDevice);

		VkPipelineLayout m_PipelineLayout;
		VkPipeline m_GraphicsPipeline;
	};
}