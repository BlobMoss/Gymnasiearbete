#pragma once

#include "../Common.h"

#include "Sprite.h"
#include "Body.h"
#include "BlockGroup.h"

namespace Collision
{
	static void CircleToCircle(Body* bodyA, Body* bodyB, float deltaTime)
	{
		glm::vec2 posA(bodyA->m_Position.x, bodyA->m_Position.z);
		glm::vec2 posB(bodyB->m_Position.x, bodyB->m_Position.z);

		float diameter = bodyA->m_ColliderRadius + bodyB->m_ColliderRadius;
		float distance = glm::distance(posA, posB);

		if (distance < diameter)
		{
			glm::vec2 direction = glm::normalize(posB - posA);
			bodyA->m_Velocity -= glm::vec3(direction.x, 0.0f, direction.y) / distance;
			bodyB->m_Velocity += glm::vec3(direction.x, 0.0f, direction.y) / distance;
		}
	}

	static void CircleToBlocks(Body* body, BlockGroup* blockGroup, float deltaTime);

	static void BlocksToBlocks(BlockGroup* blockGroupA, BlockGroup* blockGroupB, float deltaTime);
}
