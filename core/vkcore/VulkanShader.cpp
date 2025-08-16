#include "VulkanShader.h"

namespace core
{
    VulkanShader::VulkanShader(const std::string& filePath, VkDevice logicDevice)
    {
        std::filesystem::path exePath = std::filesystem::current_path();
        std::filesystem::path shaderDir = exePath / "compiled_shaders";

        auto shaderPath = shaderDir / filePath;
        auto shaderCode = utils::FileUtils::ReadShaderFile(shaderPath.string());
        CreateShaderModule(shaderCode, logicDevice);
    }

    void VulkanShader::VulkanShaderClean(VkDevice logicDevice)
    {
        vkDestroyShaderModule(logicDevice, m_Shader, nullptr);
    }

    void VulkanShader::CreateShaderModule(const std::vector<char>& code, VkDevice logicDevice)
    {
        VkShaderModuleCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
        createInfo.codeSize = code.size();
        createInfo.pCode = reinterpret_cast<const uint32_t*>(code.data());

        if (vkCreateShaderModule(logicDevice, &createInfo, nullptr, &m_Shader) != VK_SUCCESS)
        {
            throw std::runtime_error("failed to create shader module!");
        }
    }
}