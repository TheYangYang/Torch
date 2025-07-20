#pragma once
#include <pch/pch.h>

namespace core
{
	class VulkanGraphicsPipeline
	{
	public:
		VulkanGraphicsPipeline(VkDevice logicDevice, VkRenderPass renderPass);
		VkPipeline GetGraphicsPipeline() const { return m_GraphicsPipeline; }
	private:
		VkShaderModule CreateShaderModule(const std::vector<char>& code, VkDevice logicDevice);

		VkPipelineLayout m_PipelineLayout;
		VkPipeline m_GraphicsPipeline;
	};
}