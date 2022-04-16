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

class Inventory : public UISprite
{
private:
	UISprite* m_InventoryBackground;
	UIButton* m_InventoryButton;
	Image* m_ItemIcons[32];

	ItemSlot m_ItemSlots[16];
	ItemSlot m_HeldItemSlot;

	float m_GrabTime;
	bool m_FirstGrab = true;
	const float m_FirstGrabDelay = 0.70f;
	const float m_GrabDelay = 0.05f;
	
public:
	static Inventory* m_Instance;

	Item m_Items[16];
	Item m_HeldItem;

public:
	Inventory();
	~Inventory();

	bool PickUp(unsigned char type, unsigned int count);
	bool Contains(unsigned char type, unsigned int count);
	bool Spend(unsigned char type, unsigned int count);
	void UpdateSlots();

	void GameUpdate(float deltaTime) ;
	void Update(float deltaTime) override;
	void Draw() override;
};
