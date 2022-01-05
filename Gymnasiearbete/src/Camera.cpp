#include "Camera.h"

#include "Renderer.h"
#include "Input.h"

#include "gameplay/Sprite.h"

glm::vec2 Camera::m_PositionOffset = glm::vec2(40.0f, 40.0f);

glm::vec3 Camera::m_Position = glm::vec3(0.0f);
glm::vec3 Camera::m_Rotation = glm::vec3(0.0f);

float Camera::m_RotationSpeed = 1.0f;
float Camera::m_ViewAngle = 0.0f;

Sprite* Camera::m_FollowTarget;

void Camera::Update(float deltaTime)
{
    if (Input::TurnCameraLeft())
    {
        m_ViewAngle += deltaTime * m_RotationSpeed;
    }
    if (Input::TurnCameraRight())
    {
        m_ViewAngle -= deltaTime * m_RotationSpeed;
    }

    if (m_FollowTarget != nullptr) 
    {
        glm::vec3 targetPos = m_FollowTarget->m_Position;

        m_Position = targetPos;
        m_Position.x += m_PositionOffset.x * glm::sin(m_ViewAngle);
        m_Position.y += m_PositionOffset.y;
        m_Position.z += m_PositionOffset.x * glm::cos(m_ViewAngle);

        m_Rotation.x = glm::atan(m_PositionOffset.y / m_PositionOffset.x);
        m_Rotation.y = -m_ViewAngle;
    }
}

void Camera::SetFollowTarget(Sprite* target)
{
    m_FollowTarget = target;
}
Sprite* Camera::GetFollowTarget()
{
    return m_FollowTarget;
}
