#pragma once

#include "../Common.h"

const float worldWidth = 22 * 17.999f;
const float worldHeight = 22 * 17.999f;

class World
{
private:
	void GenerateIsland(glm::vec2 position);

public:
	static World* m_Instance;

public:
	World(bool generateSpawn);
	~World();

	void Update(float deltaTime);
};

