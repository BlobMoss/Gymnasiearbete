#pragma once

#include "Sprite.h"

class Water : public Sprite
{
private:
	Mesh m_Mesh;

	Mesh GenerateMesh();

public:
	Water();
	~Water();

	void Draw() override;
};

