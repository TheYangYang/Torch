#include "VulkanSurface.h"

namespace core
{
	VulkanSurface::VulkanSurface(VkInstance instance, GLFWwindow* window)
	{
		if (glfwCreateWindowSurface(instance, window, nullptr, &m_Surface) != VK_SUCCESS) {
			throw std::runtime_error("failed to create window surface!");
		}
	}
}