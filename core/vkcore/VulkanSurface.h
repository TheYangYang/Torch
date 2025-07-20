#pragma once
#include <pch/pch.h>


namespace core
{
	class VulkanSurface
	{
	public:
		VulkanSurface(VkInstance instance, GLFWwindow* window);
		VkSurfaceKHR GetSurface() const { return m_Surface; }
		
	private:
		VkSurfaceKHR m_Surface;
	};
}