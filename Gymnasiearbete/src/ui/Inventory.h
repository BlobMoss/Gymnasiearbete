#pragma once

#include "UISpriteManager.h"

#include "../gameplay/ItemData.h"

struct Item {
	unsigned char type;
	unsigned char count;
};

struct ItemSlot {
	UIButton* button;
	UIText* text;
};

class Inventory
{
private:
	UISprite* m_InventoryBackground;

	ItemSlot m_ItemSlots[16];
	ItemSlot m_HeldItemSlot;

	float m_GrabTime;
	bool m_FirstGrab = true;
	const float m_FirstGrabDelay = 0.70f;
	const float m_GrabDelay = 0.10f;
	
public:
	static Inventory* m_Instance;

	UIButton* m_InventoryButton;

	Image* m_ItemIcons[32];
	Item m_Items[16];
	Item m_HeldItem;

	Item* m_UseItem;

public:
	Inventory();
	~Inventory();

	bool PickUp(unsigned char type, unsigned int count);
	bool Contains(unsigned char type, unsigned int count);
	bool Spend(unsigned char type, unsigned int count);
	void DropAll(glm::vec3 position);
	void UpdateSlots();

	void Update(float deltaTime);
};
