#pragma once
#include <pch.h>


namespace core
{
	class VulkanInstance
	{
	public:
		VulkanInstance();
		~VulkanInstance();

		VkInstance GetInstance() const { return m_Instance; }

		void CleanInstance()
		{
			vkDestroyInstance(m_Instance, nullptr);
		}
	private:
		VkInstance m_Instance;
	};
}