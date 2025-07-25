#include "EditorCamera.h"
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>
#include <core/events/Keyboard.h>
#include <core/events/Mouse.h>

namespace core
{
    EditorCamera::EditorCamera()
    {
        UpdateView();
    }

    EditorCamera::EditorCamera(float fov, float aspectRatio, float nearClip, float farClip)
        : m_FOV(fov), m_AspectRatio(aspectRatio), m_NearClip(nearClip), m_FarClip(farClip),
        Camera(glm::perspective(glm::radians(fov), aspectRatio, nearClip, farClip))
    {
        UpdateView();
    }

    void EditorCamera::Update()
    {
        if (m_Keyboard->IsKeyPressed(KeyCode::KEY_LEFT_CONTROL))
        {
            if (m_IsOnFocus)
            {
                auto posX = m_Mouse->GetCursorPosX();
                auto posY = m_Mouse->GetCursorPosY();

                auto offsetMouse = m_Mouse->GetPositionOffset() * 0.003f;

                if (m_Mouse->IsLeftButtonPressed())
                {
                    MouseRotate(offsetMouse);
                }
                else if (m_Mouse->IsRightButtonPressed())
                {
                    MousePan(offsetMouse);
                }

                auto offsetScroll = m_Mouse->GetScrollOffsetY();
                MouseZoom(offsetScroll);

                m_Mouse->UpdateMouse();
            }
            UpdateView();
        }
    }

    void EditorCamera::SetCameraAttributes(float fov, float aspectRatio, float nearClip, float farClip)
    {
        m_FOV = fov;
        this->m_AspectRatio = aspectRatio;
        this->m_NearClip = nearClip;
        this->m_FarClip = farClip;

        m_Projection = glm::perspective(glm::radians(fov), aspectRatio, nearClip, farClip);
        UpdateView();
    }

    glm::vec3 EditorCamera::GetUpDirection() const
    {
        return glm::rotate(GetOrientation(), glm::vec3(0.0f, 1.0f, 0.0f));
    }

    glm::vec3 EditorCamera::GetRightDirection() const
    {
        return glm::rotate(GetOrientation(), glm::vec3(1.0f, 0.0f, 0.0f));
    }

    glm::vec3 EditorCamera::GetForwardDirection() const
    {
        return glm::rotate(GetOrientation(), glm::vec3(0.0f, 0.0f, -1.0f));
    }

    glm::quat EditorCamera::GetOrientation() const
    {
        return glm::quat(glm::vec3(-m_Pitch, -m_Yaw, 0.0f));
    }

    void EditorCamera::UpdateProjection()
    {
        if (m_ViewportHeight <= 0.0f)
        {
            m_ViewportWidth = 1.0f;
        }
        float aspectRatio = m_ViewportWidth / m_ViewportHeight;
        m_Projection = glm::perspective(glm::radians(m_FOV), aspectRatio, m_NearClip, m_FarClip);
    }

    void EditorCamera::UpdateView()
    {
        m_Position = CalculatePosition();
        glm::quat orientation = GetOrientation();
        m_ViewMatrix = glm::translate(glm::mat4(1.0f), m_Position) * glm::toMat4(orientation);
        m_ViewMatrix = glm::inverse(m_ViewMatrix);
    }

    void EditorCamera::MousePan(const glm::vec2& delta)
    {
        auto [xSpeed, ySpeed] = PanSpeed();
        m_FocalPoint += -GetRightDirection() * delta.x * xSpeed * m_Distance;
        m_FocalPoint += GetUpDirection() * delta.y * ySpeed * m_Distance;
    }

    void EditorCamera::MouseRotate(const glm::vec2& delta)
    {
        float yawSign = GetUpDirection().y < 0 ? -1.0f : 1.0f;
        m_Yaw += yawSign * delta.x * RotationSpeed();
        m_Pitch += delta.y * RotationSpeed();
    }

    void EditorCamera::MouseZoom(float delta)
    {
        m_Distance -= delta * ZoomSpeed();
        if (m_Distance < 1.0f)
        {
            m_FocalPoint += GetForwardDirection();
            m_Distance = 1.0f;
        }
    }

    glm::vec3 EditorCamera::CalculatePosition() const
    {
        return m_FocalPoint - GetForwardDirection() * m_Distance;
    }

    std::pair<float, float> EditorCamera::PanSpeed() const
    {
        float x = std::min(m_ViewportWidth / 1000.0f, 2.4f); // max = 2.4f
        float xFactor = 0.0366f * (x * x) - 0.1778f * x + 0.3021f;

        float y = std::min(m_ViewportHeight / 1000.0f, 2.4f); // max = 2.4f
        float yFactor = 0.0366f * (y * y) - 0.1778f * y + 0.3021f;

        return { xFactor, yFactor };
    }

    float EditorCamera::RotationSpeed() const
    {
        return 0.8f;
    }

    float EditorCamera::ZoomSpeed() const
    {
        float distance = this->m_Distance * 0.2f;
        distance = std::max(distance, 0.0f);
        float speed = distance * distance;
        speed = std::min(speed, 100.0f); // max speed = 100
        return speed;
    }

    void EditorCamera::SetIsOnFocus(bool focus)
    {
        m_IsOnFocus = focus;
    }
}
