#pragma once

#include "UISpriteManager.h"

struct Item {
	unsigned char type;
	unsigned char count;
	bool stackable;
};

class Inventory
{
private:
	static Image* m_InventoryBackground;
	static Image* m_ItemIcons[16];

	static glm::ivec2 m_IconPositions[16];

	static Item m_Items[16];
	static Item m_PickedUpItem;

	static bool m_Initialized;

private:
	static void GenerateIconMeshes();

public:
	static void Initialize();

	static void Update(float deltaTime);
	static void Draw();
};
