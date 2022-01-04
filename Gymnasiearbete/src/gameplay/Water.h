#pragma once

#include "Sprite.h"

class Water : public Sprite
{
private:
	bool m_Loaded[256][256];

	Mesh m_Mesh;

	Mesh GenerateMesh();

public:
	Water();
	~Water();

	void Update(float deltaTime) override;
	void Draw() override;
};

