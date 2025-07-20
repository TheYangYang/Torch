#include "VulkanLogicDevice.h"
#include "VulkanUtils.h"

namespace core
{
	VulkanLogicDevice::VulkanLogicDevice(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface)
	{
		QueueFamilyIndices indices = VulkanUtils::FindQueueFamilies(physicalDevice, surface);

		std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
		std::set<uint32_t> uniqueQueueFamilies = { indices.graphicsFamily.value(), indices.presentFamily.value() };

		float queuePriority = 1.0f;
		for (uint32_t queueFamily : uniqueQueueFamilies)
		{
			VkDeviceQueueCreateInfo queueCreateInfo{};
			queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
			queueCreateInfo.queueFamilyIndex = queueFamily;
			queueCreateInfo.queueCount = 1;
			queueCreateInfo.pQueuePriorities = &queuePriority;
			queueCreateInfos.push_back(queueCreateInfo);
		}

		VkPhysicalDeviceFeatures deviceFeatures{};

		VkDeviceCreateInfo createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;

		createInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());
		createInfo.pQueueCreateInfos = queueCreateInfos.data();

		createInfo.pEnabledFeatures = &deviceFeatures;

		createInfo.enabledExtensionCount = static_cast<uint32_t>(deviceExtensions.size());
		createInfo.ppEnabledExtensionNames = deviceExtensions.data();


		if (enableValidationLayers) 
		{
			createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
			createInfo.ppEnabledLayerNames = validationLayers.data();
		}
		else
		{
			createInfo.enabledLayerCount = 0;
		}

		if (vkCreateDevice(physicalDevice, &createInfo, nullptr, &m_LogicDevice) != VK_SUCCESS) 
		{
			throw std::runtime_error("failed to create logical device!");
		}

		vkGetDeviceQueue(m_LogicDevice, indices.graphicsFamily.value(), 0, &m_GraphicsQueue);
		vkGetDeviceQueue(m_LogicDevice, indices.presentFamily.value(), 0, &m_PresentQueue);


		CreatePipelineCache();
		CreateDescriptorPool();
	}

	VulkanLogicDevice::~VulkanLogicDevice()
	{
		if (m_DescriptorPool != VK_NULL_HANDLE)
			vkDestroyDescriptorPool(m_LogicDevice, m_DescriptorPool, nullptr);

		if (m_PipelineCache != VK_NULL_HANDLE)
			vkDestroyPipelineCache(m_LogicDevice, m_PipelineCache, nullptr);

		if (m_LogicDevice != VK_NULL_HANDLE)
			vkDestroyDevice(m_LogicDevice, nullptr);
	}


	void VulkanLogicDevice::CreatePipelineCache()
	{
		VkPipelineCacheCreateInfo cacheInfo{};
		cacheInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_CACHE_CREATE_INFO;
		cacheInfo.initialDataSize = 0;
		cacheInfo.pInitialData = nullptr;

		if (vkCreatePipelineCache(m_LogicDevice, &cacheInfo, nullptr, &m_PipelineCache) != VK_SUCCESS)
		{
			throw std::runtime_error("Failed to create pipeline cache!");
		}
	}

	void VulkanLogicDevice::CreateDescriptorPool()
	{
		VkDescriptorPoolSize pool_sizes[] =
		{
			{ VK_DESCRIPTOR_TYPE_SAMPLER, 1000 },
			{ VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1000 },
			{ VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE, 1000 },
			{ VK_DESCRIPTOR_TYPE_STORAGE_IMAGE, 1000 },
			{ VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER, 1000 },
			{ VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER, 1000 },
			{ VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1000 },
			{ VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, 1000 },
			{ VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC, 1000 },
			{ VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC, 1000 },
			{ VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT, 1000 }
		};

		VkDescriptorPoolCreateInfo poolInfo{};
		poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
		poolInfo.flags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;
		poolInfo.maxSets = 1000 * static_cast<uint32_t>(std::size(pool_sizes));
		poolInfo.poolSizeCount = static_cast<uint32_t>(std::size(pool_sizes));
		poolInfo.pPoolSizes = pool_sizes;

		if (vkCreateDescriptorPool(m_LogicDevice, &poolInfo, nullptr, &m_DescriptorPool) != VK_SUCCESS)
		{
			throw std::runtime_error("Failed to create descriptor pool!");
		}
	}

}

