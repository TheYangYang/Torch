#pragma once
#include <pch/pch.h>


namespace core
{
	class VulkanSurface
	{
	public:
		VulkanSurface(VkInstance instance, GLFWwindow* window);
		VkSurfaceKHR GetSurface() const { return m_Surface; }
		void CleanSurface(VkInstance instance)
		{
			vkDestroySurfaceKHR(instance, m_Surface, nullptr);
		}
	private:
		VkSurfaceKHR m_Surface;
	};
}