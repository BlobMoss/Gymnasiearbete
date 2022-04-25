#include "Crafting.h"

#include "../Input.h"

Crafting* Crafting::m_Instance = nullptr;

Crafting::Crafting()
	: m_OpenedCategory(-1), m_OpenedRecipe(-1), m_MostCategories(0)
{
	m_Instance = this;

	// MaterialRecipes: 
	std::vector<Recipe> materialRecipes =
	{
		{ 
			{ PLANKS, 4 }, 
			{ { GRASS, 1 } }
		}
	};

	// FoodRecipes: 
	std::vector<Recipe> foodRecipes =
	{
		{
			{ GRASS, 1 },
			{ { PLANKS, 4 } }
		}
	};

	// BuildingRecipes: 
	std::vector<Recipe> buildingRecipes =
	{
		{
			{ MAST, 1 },
			{ { PLANKS, 20 } }
		},
		{
			{ HELM, 1 },
			{ { PLANKS, 10 }, { SAND, 2 } }
		},
		{
			{ CANNON, 1 },
			{ { PLANKS, 5 }, { CANNONBALL, 10 } }
		}
	};

	// CombatRecipes: 
	std::vector<Recipe> combatRecipes =
	{
		{
			{ CANNONBALL, 3 },
			{ { SAND, 1 } }
		}
	};

	m_Recipes.push_back(materialRecipes);
	m_Recipes.push_back(foodRecipes);
	m_Recipes.push_back(buildingRecipes);
	m_Recipes.push_back(combatRecipes);

	for (auto& category : m_Recipes)
	{
		if (category.size() > m_MostCategories) m_MostCategories = category.size();
	}

	for (unsigned int i = 0; i < 4; i++)
	{
		m_CraftIcons[i] = new UIButton(new Image("res/images/craft_icons.png", 0, i * 24, 19, 24));
		m_CraftIcons[i]->m_Position = glm::vec2(1, i * 26 + 129);
		m_CraftIcons[i]->m_SortingOrder = 0.61f;
		UISpriteManager::AddSprite(m_CraftIcons[i]);
	}

	m_Highlighted = new UISprite(new Image("res/images/craft_highlighted.png"));
	m_Highlighted->m_Active = false;
	m_Highlighted->m_SortingOrder = 0.60f;
	UISpriteManager::AddSprite(m_Highlighted);
	
	m_FrameTop = new UISprite(new Image("res/images/craft_frame_top.png"));
	m_FrameTop->m_Active = false;
	m_FrameTop->m_Position = glm::vec2(39, 254);
	m_FrameTop->m_SortingOrder = 0.55f;
	UISpriteManager::AddSprite(m_FrameTop);

	m_Banner = new UISprite(new Image("res/images/craft_banner.png"));
	m_Banner->m_Active = false;
	m_Banner->m_SortingOrder = 0.55f;
	UISpriteManager::AddSprite(m_Banner);

	Image* frameImage = new Image("res/images/craft_frame.png");

	for (unsigned int i = 0; i < m_MostCategories; i++)
	{
		glm::vec2 pos(39, 220 - (34 * i));

		UIButton* button = new UIButton(frameImage, 38, 28);
		button->m_Position = pos;
		button->m_SortingOrder = 0.55f;
		UISpriteManager::AddSprite(button);

		UISprite* sprite = new UISprite();
		sprite->m_Position = pos + glm::vec2(9, 4);
		sprite->m_SortingOrder = 0.60f;
		UISpriteManager::AddSprite(sprite);

		UIText* text = new UIText(" ");
		text->m_SortingOrder = 0.65f;
		text->m_Position = pos + glm::vec2(9, 4);
		UISpriteManager::AddSprite(text);

		m_Frames.push_back({ button, sprite, text });
	}

	for (unsigned int i = 0; i < 3; i++)
	{
		UISprite* sprite = new UISprite();
		sprite->m_SortingOrder = 0.60f;
		UISpriteManager::AddSprite(sprite);

		UIText* text = new UIText(" ");
		text->m_SortingOrder = 0.65f;
		UISpriteManager::AddSprite(text);

		m_Requirements.push_back({ sprite, text });
	}

	UpdateSlots();
}
Crafting::~Crafting()
{

}

void Crafting::UpdateSlots()
{
	m_Highlighted->m_Active = false;

	m_FrameTop->m_Active = false;

	m_Banner->m_Active = false;

	for (unsigned int i = 0; i < 4; i++)
	{
		m_CraftIcons[i]->m_Position.x = 1;
	}
	for (unsigned int i = 0; i < m_MostCategories; i++)
	{
		m_Frames[i].button->m_Active = false;
		m_Frames[i].sprite->m_Active = false;
		m_Frames[i].text->m_Active = false;
	}
	for (unsigned int i = 0; i < 3; i++)
	{
		m_Requirements[i].sprite->m_Active = false;
		m_Requirements[i].text->m_Active = false;
	}
	if (m_OpenedCategory != -1)
	{
		m_Highlighted->m_Active = true;
		m_Highlighted->m_Position = glm::vec2(0, m_OpenedCategory * 26 + 126);
		
		m_CraftIcons[m_OpenedCategory]->m_Position.x = 3;

		m_FrameTop->m_Active = true;

		auto category = m_Recipes[3 - m_OpenedCategory];

		for (unsigned int i = 0; i < category.size(); i++)
		{
			ItemFrame frame = m_Frames[i];

			frame.button->m_Active = true;

			frame.sprite->m_Image = Inventory::m_Instance->m_ItemIcons[category[i].product.type - 1];
			frame.sprite->m_Active = true;

			if (category[i].product.count > 1)
			{
				frame.text->m_Active = true;
				frame.text->SetText(std::to_string(category[i].product.count));
			}
		}
		if (m_OpenedRecipe != -1)
		{
			m_Banner->m_Position = glm::vec2(77, 223 - m_OpenedRecipe * 34);
			m_Banner->m_Active = true;

			for (unsigned int ii = 0; ii < category[m_OpenedRecipe].ingredients.size(); ii++)
			{
				IngridientRequirement requirement = m_Requirements[ii];

				glm::ivec2 pos = m_Frames[m_OpenedRecipe].sprite->m_Position + glm::ivec2(ii * 26 + 38, 0);

				requirement.sprite->m_Active = true;
				requirement.sprite->m_Image = Inventory::m_Instance->m_ItemIcons[category[m_OpenedRecipe].ingredients[ii].type - 1];
				requirement.sprite->m_Position = pos;

				if (category[m_OpenedRecipe].ingredients[ii].count > 1)
				{
					requirement.text->m_Active = true;
					requirement.text->SetText(std::to_string(category[m_OpenedRecipe].ingredients[ii].count));
					requirement.text->m_Position = pos;
				}
			}
		}
	}
}

void Crafting::Update(float deltaTime)
{
	m_HoveringFrames = false;

	for (unsigned int i = 0; i < 4; i++)
	{
		UIButton* button = m_CraftIcons[i];
		if (button->Hover())
		{
			if (Input::MouseButtonDown(MOUSE_BUTTON_LEFT))
			{
				m_OpenedCategory = m_OpenedCategory == i ? -1 : i;
				UpdateSlots();
			}
			m_HoveringFrames = true;
		}
	}

	unsigned int oldOpenedRecipe = m_OpenedRecipe;
	m_OpenedRecipe = -1;
	if (m_OpenedCategory != -1)
	{
		auto category = m_Recipes[3 - m_OpenedCategory];

		for (unsigned int i = 0; i < category.size(); i++)
		{
			ItemFrame frame = m_Frames[i];
			if (frame.button->Hover())
			{
				m_OpenedRecipe = i;

				m_HoveringFrames = true;

				if (Input::MouseButtonDown(MOUSE_BUTTON_LEFT) || Input::MouseButtonHeld(MOUSE_BUTTON_RIGHT))
				{
					if (m_GrabTime <= 0.0f || m_FirstGrab || Input::MouseButtonDown(MOUSE_BUTTON_LEFT))
					{
						Recipe recipe = category[m_OpenedRecipe];

						bool sameType = Inventory::m_Instance->m_HeldItem.type == recipe.product.type;
						bool heldEmpty = Inventory::m_Instance->m_HeldItem.count == 0;
						bool canStack = isStackable[recipe.product.type] && Inventory::m_Instance->m_HeldItem.count > 0;
						if ((sameType && canStack) || heldEmpty)
						{
							bool hasIngredients = true;
							for (auto& requirement : recipe.ingredients)
							{
								if (!Inventory::m_Instance->Contains(requirement.type, requirement.count))
								{
									hasIngredients = false;
								}
							}
							if (hasIngredients)
							{
								for (auto& requirement : recipe.ingredients)
								{
									Inventory::m_Instance->Spend(requirement.type, requirement.count);
								}

								Inventory::m_Instance->m_HeldItem.type = recipe.product.type;
								Inventory::m_Instance->m_HeldItem.count += recipe.product.count;

								Inventory::m_Instance->UpdateSlots();

								if (!m_FirstGrab) m_GrabTime = m_GrabDelay;
								m_FirstGrab = false;
							}
						}
					}
				}
				else
				{
					m_FirstGrab = true;
					m_GrabTime = m_FirstGrabDelay;
				}
			}
		}
	}

	m_GrabTime -= deltaTime;

	if (m_OpenedRecipe != oldOpenedRecipe) UpdateSlots();
}