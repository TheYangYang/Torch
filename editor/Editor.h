#pragma once
#include <pch/pch.h>
#include "EditorModule.h"
#include <core/graphics/TorchVulkanContext.h>
#include <core/platform/TorchWindow.h>

namespace editor
{
    static void check_vk_result(VkResult err)
    {
        if (err == VK_SUCCESS)
            return;
        fprintf(stderr, "[vulkan] Error: VkResult = %d\n", err);
        if (err < 0)
            abort();
    }

    enum class EditorType
    {
        None,
        Viewport,
        ScenePanel,
        Settings,
        EntityPanel,
        Scene,
        FileBrowser,
        Environment,
    };

    class Editor
    {
    public:
        Editor() = default;
        static void AddModule(EditorType type, std::unique_ptr<EditorModule> module);
        static void Render();
        static void SetUpImGui();
        static void ImGuiBegin();
        static void ImGuiEnd();
        static void OpenSceneFile();
        static void SaveSceneFile();
        static std::unique_ptr<EditorModule>& GetEditorModule(EditorType type) { return m_EditorModules[type]; }
    private:
        static void DockSpace();
        static void RenderNavbar();
        static std::unordered_map<EditorType, std::unique_ptr<EditorModule>> m_EditorModules;
        static core::Window* m_WindowPtr;

        static bool fullscreen;
        static bool isOpen;

        static uint32_t m_NavbarHeight;
        static bool m_FullScreen;
        static void KeyboardShortCut();


        static ImGui_ImplVulkanH_Window g_MainWindowData;
        static ImGui_ImplVulkanH_Window* wd;
        static bool g_SwapChainRebuild;

        static void FramePresent(ImGui_ImplVulkanH_Window* wd, VkQueue queue)
        {
            if (g_SwapChainRebuild)
                return;
            VkSemaphore render_complete_semaphore = wd->FrameSemaphores[wd->SemaphoreIndex].RenderCompleteSemaphore;
            VkPresentInfoKHR info = {};
            info.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
            info.waitSemaphoreCount = 1;
            info.pWaitSemaphores = &render_complete_semaphore;
            info.swapchainCount = 1;
            info.pSwapchains = &wd->Swapchain;
            info.pImageIndices = &wd->FrameIndex;
            VkResult err = vkQueuePresentKHR(queue, &info);
            if (err == VK_ERROR_OUT_OF_DATE_KHR || err == VK_SUBOPTIMAL_KHR)
                g_SwapChainRebuild = true;
            if (err == VK_ERROR_OUT_OF_DATE_KHR)
                return;
            if (err != VK_SUBOPTIMAL_KHR)
                check_vk_result(err);
            wd->SemaphoreIndex = (wd->SemaphoreIndex + 1) % wd->SemaphoreCount; // Now we can use the next set of semaphores
        }

        static void FrameRender(ImGui_ImplVulkanH_Window* wd, ImDrawData* draw_data, VkDevice logicDevice, VkQueue queue)
        {
            VkSemaphore image_acquired_semaphore = wd->FrameSemaphores[wd->SemaphoreIndex].ImageAcquiredSemaphore;
            VkSemaphore render_complete_semaphore = wd->FrameSemaphores[wd->SemaphoreIndex].RenderCompleteSemaphore;
            VkResult err = vkAcquireNextImageKHR(logicDevice, wd->Swapchain, UINT64_MAX, image_acquired_semaphore, VK_NULL_HANDLE, &wd->FrameIndex);
            if (err == VK_ERROR_OUT_OF_DATE_KHR || err == VK_SUBOPTIMAL_KHR)
                g_SwapChainRebuild = true;
            if (err == VK_ERROR_OUT_OF_DATE_KHR)
                return;
            if (err != VK_SUBOPTIMAL_KHR)
                check_vk_result(err);

            ImGui_ImplVulkanH_Frame* fd = &wd->Frames[wd->FrameIndex];
            {
                err = vkWaitForFences(logicDevice, 1, &fd->Fence, VK_TRUE, UINT64_MAX);    // wait indefinitely instead of periodically checking
                check_vk_result(err);

                err = vkResetFences(logicDevice, 1, &fd->Fence);
                check_vk_result(err);
            }
            {
                err = vkResetCommandPool(logicDevice, fd->CommandPool, 0);
                check_vk_result(err);
                VkCommandBufferBeginInfo info = {};
                info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
                info.flags |= VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
                err = vkBeginCommandBuffer(fd->CommandBuffer, &info);
                check_vk_result(err);
            }
            {
                VkRenderPassBeginInfo info = {};
                info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
                info.renderPass = wd->RenderPass;
                info.framebuffer = fd->Framebuffer;
                info.renderArea.extent.width = wd->Width;
                info.renderArea.extent.height = wd->Height;
                info.clearValueCount = 1;
                info.pClearValues = &wd->ClearValue;
                vkCmdBeginRenderPass(fd->CommandBuffer, &info, VK_SUBPASS_CONTENTS_INLINE);
            }

            // Record dear imgui primitives into command buffer
            ImGui_ImplVulkan_RenderDrawData(draw_data, fd->CommandBuffer);

            // Submit command buffer
            vkCmdEndRenderPass(fd->CommandBuffer);
            {
                VkPipelineStageFlags wait_stage = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
                VkSubmitInfo info = {};
                info.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
                info.waitSemaphoreCount = 1;
                info.pWaitSemaphores = &image_acquired_semaphore;
                info.pWaitDstStageMask = &wait_stage;
                info.commandBufferCount = 1;
                info.pCommandBuffers = &fd->CommandBuffer;
                info.signalSemaphoreCount = 1;
                info.pSignalSemaphores = &render_complete_semaphore;

                err = vkEndCommandBuffer(fd->CommandBuffer);
                check_vk_result(err);
                err = vkQueueSubmit(queue, 1, &info, fd->Fence);
                check_vk_result(err);
            }
        }
    };
}