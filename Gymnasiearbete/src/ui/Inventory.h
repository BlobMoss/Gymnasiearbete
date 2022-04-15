#pragma once

#include "UISpriteManager.h"

struct Item {
	unsigned char type;
	unsigned char count;
	bool stackable;
};

struct ItemSlot {
	UIButton* button;
	UIText* text;
};


class Inventory : public UISprite
{
private:
	UISprite* m_InventoryBackground;
	Image* m_ItemIcons[32];

	ItemSlot m_ItemSlots[16];
	ItemSlot m_HeldItemSlot;

	static Item m_Items[16];
	static Item m_HeldItem;

public:
	Inventory();
	~Inventory();

	void UpdateSlots();

	void Update(float deltaTime) override;
	void Draw() override;
};
