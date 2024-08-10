#include "Mouse.h"
#include <mutex>

namespace core
{
    std::unique_ptr<Mouse> Mouse::s_Instance = nullptr;

    Mouse& Mouse::GetInstance()
    {
        static std::mutex mutex;
        if (!s_Instance)
        {
            std::lock_guard<std::mutex> lock(mutex);
            if (!s_Instance)
            {
                s_Instance = std::unique_ptr<Mouse>(new Mouse());
            }
        }
        return *s_Instance;
    }

    void Mouse::OnEvent(MousePressEvent& event)
    {
        // Handle mouse press event
        switch (static_cast<MouseButtonType>(event.GetButton()))
        {
        case MouseButtonType::LEFT:
            TORCH_LOG_DEBUG("[{}:{}], left button clicked.", __FILE__, __LINE__);
            m_LeftButton = true;
            break;
        case MouseButtonType::RIGHT:
            TORCH_LOG_DEBUG("[{}:{}], right button clicked.", __FILE__, __LINE__);
            m_RightButton = true;
            break;
        }
    }

    void Mouse::OnEvent(MouseReleaseEvent& event)
    {
        // Handle mouse release event
        switch (static_cast<MouseButtonType>(event.GetButton()))
        {
        case MouseButtonType::LEFT:
            m_LeftButton = false;
            break;
        case MouseButtonType::RIGHT:
            m_RightButton = false;
            break;
        }
    }

    void Mouse::OnEvent(MouseMoveEvent& event)
    {
        // Handle mouse move event
        m_CursorPosX = event.GetCursorPosX();
        m_CursorPosY = event.GetCursorPosY();
    }

    void Mouse::OnEvent(MouseScrollEvent& event)
    {
        m_ScrollOffsetY = event.GetOffsetY();
        TORCH_LOG_DEBUG("[{}:{}], mouse scrolled. Scroll Y: {}", __FILE__, __LINE__, event.GetOffsetY());
    }
}
