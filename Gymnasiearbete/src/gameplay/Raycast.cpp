#include "Raycast.h"

#include "SpriteManager.h"

const float Raycast::step = 0.05f;

std::vector<BlockGroup*>* Raycast::m_BlockGroups = &SpriteManager::m_BlockGroups;

RayHit Raycast::FireRay(glm::vec3 position, glm::vec3 direction)
{
	for (auto& blockGroup : *m_BlockGroups)
	{
		glm::vec3 bgPos = blockGroup->m_Position;
		float bgRot = blockGroup->m_Rotation.y;

		glm::vec3 point = position + glm::vec3(0.0f, 0.5f, 0.0f);

		glm::ivec3 lastEmpty = glm::ivec3(0);

		point += direction * ((position.y - 2.0f) / -direction.y);

		while (point.y > -1.0)
		{
			point += direction * step;

			glm::vec3 localPoint = point - bgPos;

			localPoint = glm::vec3(
				localPoint.x * glm::cos(bgRot) - localPoint.z * glm::sin(bgRot),
				localPoint.y,
				localPoint.x * glm::sin(bgRot) + localPoint.z * glm::cos(bgRot)
			);

			glm::ivec3 blockPos(round(localPoint.x), round(localPoint.y), round(localPoint.z));

			if (blockGroup->GetBlock(blockPos) == EMPTY)
			{
				lastEmpty = blockPos;
			}
			else
			{
				return { blockGroup, lastEmpty, blockPos };
			}
		}
	}

	return { nullptr };
}