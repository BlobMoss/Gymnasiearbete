#include "Camera.h"

#include "Renderer.h"

#include "Sprite.h"

Camera::Camera()
    : m_Position(glm::vec3(0.0f)), m_Rotation(glm::vec3(0.0f)), 
    m_PositionOffset(glm::vec2(40.0f, 40.0f)), m_ViewAngle(0.0f),
    m_FollowTarget(nullptr)
{
    
}
Camera::~Camera()
{

}

void Camera::Update(float deltaTime)
{
    glm::vec3 targetPos = m_FollowTarget->GetPosition();

    m_Position = targetPos;
    m_Position.x += m_PositionOffset.x * glm::sin(m_ViewAngle);
    m_Position.y += m_PositionOffset.y;
    m_Position.z += m_PositionOffset.x * glm::cos(m_ViewAngle);

    m_Rotation.x = glm::atan(m_PositionOffset.y / m_PositionOffset.x);
    m_Rotation.y = -m_ViewAngle;

    m_ViewAngle += deltaTime * 0.5f;
}

void Camera::SetFollowTarget(Sprite& target)
{
    m_FollowTarget = &target;
}

void Camera::SetPosition(glm::vec3 p)
{
    m_Position = p;
}
glm::vec3 Camera::GetPosition() const
{
    return m_Position;
}
void Camera::SetRotation(glm::vec3 r)
{
    m_Rotation = r;
}
glm::vec3 Camera::GetRotation() const
{
    return m_Rotation;
}