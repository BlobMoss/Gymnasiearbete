#pragma once

#include "UISpriteManager.h"
#include "Inventory.h"

#include "../gameplay/ItemData.h"

struct Recipe {
	Item product;
	std::vector<Item> ingredients;
};

struct ItemFrame {
	UIButton* button;
	UISprite* sprite;
	UIText* text;
};

struct IngridientRequirement {
	UISprite* sprite;
	UIText* text;
};

class Crafting
{
private:
	std::vector<std::vector<Recipe>> m_Recipes;

	UIButton* m_CraftIcons[4];
	UISprite* m_Highlighted;
	UISprite* m_FrameTop;
	UISprite* m_Banner;
	std::vector<ItemFrame> m_Frames;
	std::vector<IngridientRequirement> m_Requirements;

	unsigned int m_OpenedCategory;
	unsigned int m_OpenedRecipe;
	unsigned int m_MostCategories;

	float m_GrabTime;
	bool m_FirstGrab = true;
	const float m_FirstGrabDelay = 0.70f;
	const float m_GrabDelay = 0.10f;

public:
	static Crafting* m_Instance;

	bool m_HoveringFrames;

public:
	Crafting();
	~Crafting();

	void UpdateSlots();

	void Update(float deltaTime);
};
