#pragma once
#include <pch/pch.h>

namespace core
{
	class VulkanShader
	{
	public:
		VulkanShader() = default;
		VulkanShader(const std::string & filePath, VkDevice logicDevice);
		VkShaderModule GetShaderModule() const { return m_Shader; }
		void VulkanShaderClean(VkDevice logicDevice);
	private:
		void CreateShaderModule(const std::vector<char>& code, VkDevice logicDevice);
		VkShaderModule m_Shader;
	};
}