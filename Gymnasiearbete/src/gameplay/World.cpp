#include "World.h"

#include "glm/gtc/noise.hpp"

#include "SpriteManager.h"

World* World::m_Instance = nullptr;

World::World(bool generateSpawn)
{
	World::m_Instance = this;

	if (generateSpawn)
	{
		GenerateIsland(glm::vec2(-28.0f, -25.0f));
		GenerateIsland(glm::vec2(28.0f, -25.0f));
		GenerateIsland(glm::vec2(0.0f, 25.0f));
	}
}
World::~World()
{

}

void World::GenerateIsland(glm::vec2 position)
{
	BlockGroup* blockGroup = new BlockGroup();

	blockGroup->m_Position = glm::vec3(position.x, 0.0f, position.y);
	//blockGroup->m_Rotation = glm::vec3(0.0f, randf() * glm::pi<float>() * 2.0f, 0.0f);

	glm::vec2 offset(randf() * 10000.0f, randf() * 10000.0f);

	for (int z = -32; z < 32; z++)
	{
		for (int x = -32; x < 32; x++)
		{
			float value = glm::perlin(glm::vec2(x / 22.0f, z / 22.0f) + offset) * 10.0f;

			value -= (pow(x, 2.0f) + pow(z, 2.0f)) / 80.0f;

			if (value > 0.0f)
			{
				blockGroup->SetBlock(glm::ivec3(x, 0, z), SAND);

				if (randf() > 0.985f && value <= 1.0f)
				{
					blockGroup->SetBlock(glm::ivec3(x, 1, z), FERN);
				}
			}
			if (value > 1.0f)
			{
				blockGroup->SetBlock(glm::ivec3(x, 0, z), GRASS);

				if (randf() > 0.97f && value <= 2.6f)
				{
					float radius = (randf() * 2.0f) + 0.5f;

					for (float ox = -radius; ox < radius; ox++)
					{
						for (float oz = -radius; oz < radius; oz++)
						{
							if (blockGroup->GetBlock(glm::ivec3(x + ox, 0, z + oz)) == EMPTY) continue;
							if (blockGroup->GetBlock(glm::ivec3(x + ox, 1, z + oz)) != EMPTY) continue;

							blockGroup->SetBlock(glm::ivec3(x + ox, 1, z + oz), FERN);
						}
					}
				}
				else if (randf() > 0.967f && value <= 2.6f)
				{
					blockGroup->SetBlock(glm::ivec3(x, 1, z), TREE);
				}
			}
			if (value > 2.6f)
			{
				blockGroup->SetBlock(glm::ivec3(x, 0, z), STONE);
				blockGroup->SetBlock(glm::ivec3(x, 1, z), GRASS);
			}
			if (value > 3.3f)
			{
				blockGroup->SetBlock(glm::ivec3(x, 1, z), STONE);

				if (randf() > 0.97f)
				{
					float radius = (randf() * 1.4f) + 0.5f;

					for (float ox = -radius; ox < radius; ox++)
					{
						for (float oz = -radius; oz < radius; oz++)
						{
							if (blockGroup->GetBlock(glm::ivec3(x + ox, 1, z + oz)) != STONE) continue;

							blockGroup->SetBlock(glm::ivec3(x + ox, 1, z + oz), IRONORE);
						}
					}
				}
				else if (randf() > 0.98f)
				{
					float radius = (randf() * 0.6f) + 0.4f;

					for (float ox = -radius; ox < radius; ox++)
					{
						for (float oz = -radius; oz < radius; oz++)
						{
							if (blockGroup->GetBlock(glm::ivec3(x + ox, 1, z + oz)) != STONE) continue;

							blockGroup->SetBlock(glm::ivec3(x + ox, 1, z + oz), GOLDORE);
						}
					}
				}
			}
		}
	}

	SpriteManager::AddSprite(blockGroup);
}

void World::Update(float deltaTime)
{

}