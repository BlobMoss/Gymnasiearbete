#pragma once

#include "../Common.h"

#include "BlockGroup.h"

struct RayHit {
	BlockGroup* blockGroup;
	glm::ivec3 lastEmpty;
	glm::ivec3 firstBlock;
};

class Raycast
{
private:
	static const float step;

	static std::vector<BlockGroup*>* m_BlockGroups;

public:

	static RayHit FireRay(glm::vec3 position, glm::vec3 direction);
};