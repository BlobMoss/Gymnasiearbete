#include "Camera.h"

#include "Renderer.h"
#include "Input.h"

#include "gameplay/Sprite.h"

#include "gameplay/Raycast.h"

glm::vec2 Camera::m_PositionOffset = glm::vec2(40.0f, 40.0f);

glm::vec3 Camera::m_Position = glm::vec3(0.0f, 40.0f, 40.0f);
glm::vec3 Camera::m_Rotation = glm::vec3(0.785398f, 0.0f, 0.0f);

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

RayHit Camera::RayFromScreen(glm::vec2 point)
{
    float pointX = point.x / (referenceWidth * Renderer::pixelSize * 0.5f) - 1.0f;
    float pointY = point.y / (referenceHeight * Renderer::pixelSize * 0.5f) - 1.0f;

    glm::vec3 cameraDir = glm::vec3(0.0f, 0.0f, 1.0f);

    glm::mat4 proj = glm::perspective(glm::radians(30.0f), (GLfloat)referenceWidth / (GLfloat)referenceHeight, nearPlane, farPlane);
    glm::mat4 view = glm::mat4(1.0f);

    view = glm::rotate(view, Camera::m_Rotation.x, glm::vec3(1, 0, 0));
    view = glm::rotate(view, Camera::m_Rotation.y, glm::vec3(0, 1, 0));
    view = glm::rotate(view, Camera::m_Rotation.z, glm::vec3(0, 0, 1));

    glm::mat4 invVP = glm::inverse(proj * view);
    glm::vec4 screenPos = glm::vec4(pointX, -pointY, 1.0f, 1.0f);
    glm::vec4 worldPos = invVP * screenPos;

    glm::vec3 dir = glm::normalize(glm::vec3(worldPos));

    return Raycast::FireRay(m_Position, dir);
}

void Camera::SetFollowTarget(Sprite* target)
{
    m_FollowTarget = target;
}
Sprite* Camera::GetFollowTarget()
{
    return m_FollowTarget;
}
