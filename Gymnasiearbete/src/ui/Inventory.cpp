#include "Inventory.h"

#include "../Input.h"
#include "../gameplay/SpriteManager.h"
#include "Crafting.h"

Inventory* Inventory::m_Instance = nullptr;

Inventory::Inventory()
{
	m_Instance = this;

	m_InventoryBackground = new UISprite(new Image("res/images/inventory.png"));
	m_InventoryBackground->m_SortingOrder = 0.55f;
	UISpriteManager::AddSprite(m_InventoryBackground);

	m_InventorySelection = new UISprite(new Image("res/images/inventory_highlighted.png"));
	m_InventorySelection->m_SortingOrder = 0.56f;
	UISpriteManager::AddSprite(m_InventorySelection);

	m_InventoryButton = new UIButton(26 * 16, 26);
	m_InventoryButton->m_Position = glm::ivec2(115, 0);
	UISpriteManager::AddSprite(m_InventoryButton);

	for (unsigned int i = 0; i < 32; i++)
	{
		m_ItemIcons[i] = new Image("res/images/item_icons.png", (i % 16) * 20, (i < 16) * 20, 20, 20);
	}

	for (unsigned int i = 0; i < 17; i++)
	{
		UIButton* button = new UIButton(20, 20);
		button->m_Position = glm::ivec2(115 + (i * 26), 3);
		UISpriteManager::AddSprite(button);

		UIText* text = new UIText(" ");
		text->m_Position = button->m_Position;
		UISpriteManager::AddSprite(text);

		if (i < 16) m_ItemSlots[i] = { button, text }; else m_HeldItemSlot = { button, text };
	}

	for (unsigned int i = 0; i < 16; i++)
	{
		m_Items[i] = { 0, 0 };
	}
	m_HeldItem = { 0, 0 };

	m_Items[0].type = PLANKS;
	m_Items[0].count = 40;

	m_Items[1].type = CANNONBALL;
	m_Items[1].count = 20;

	m_Items[3].type = CUTLASS;
	m_Items[3].count = 1;

	UpdateSlots();
}

Inventory::~Inventory()
{

}

bool Inventory::PickUp(unsigned char type, unsigned int count)
{
	int firstEmpty = -1;
	for (unsigned int i = 0; i < 17; i++)
	{
		Item* item = i != 16 ? &m_Items[i] : &m_HeldItem;
		if (item->count == 0 && firstEmpty == -1)
		{
			firstEmpty = i;
		}
		if (item->count > 0 && type == item->type && isStackable[type])
		{
			item->count += count;
			UpdateSlots();
			return true;
		}
	}

	if (firstEmpty != -1)
	{
		Item* item = firstEmpty != 16 ? &m_Items[firstEmpty] : &m_HeldItem;

		item->type = type;
		item->count = count;
		UpdateSlots();
		return true;
	}

	return false;
}

bool Inventory::Contains(unsigned char type, unsigned int count)
{
	unsigned int total = 0;

	for (unsigned int i = 0; i < 17; i++)
	{
		Item* item = i != 16 ? &m_Items[i] : &m_HeldItem;
		if (item->type == type)
		{
			total += item->count;
		}
	}

	return total >= count;
}

bool Inventory::Spend(unsigned char type, unsigned int count)
{
	if (!Contains(type, count)) return false;

	for (unsigned int i = 0; i < 17; i++)
	{
		Item* item = i != 16 ? &m_Items[i] : &m_HeldItem;
		if (item->type == type)
		{
			while (item->count > 0 && count > 0)
			{
				item->count--;
				count--;
			}
		}
	}

	UpdateSlots();
	return true;
}

void Inventory::DropAll(glm::vec3 position)
{
	for (unsigned int i = 0; i < 17; i++)
	{
		Item* item = i != 16 ? &m_Items[i] : &m_HeldItem;

		if (item->count > 0)
		{
			DroppedItem* droppedItem = new DroppedItem(item->type, item->count);
			float angle = glm::pi<float>() * 2.0f * (i / 16.0f);
			glm::vec2 dir(cos(angle), sin(angle));
			droppedItem->m_CanBePickedUp = false;
			droppedItem->m_Position = position + glm::vec3(dir.x * 0.5f, 1.0f, dir.y * 0.5f);
			droppedItem->m_KnockBackVelocity = glm::vec3(dir.x, 0.0f, dir.y) * 3.0f;
			droppedItem->m_Velocity.y = 4.0f;
			SpriteManager::AddSprite(droppedItem);

			item->count = 0;
		}
	}

	UpdateSlots();
}

void Inventory::UpdateSlots()
{
	for (unsigned int i = 0; i < 17; i++)
	{
		ItemSlot slot = i != 16 ? m_ItemSlots[i] : m_HeldItemSlot;
		Item item = i != 16 ? m_Items[i] : m_HeldItem;

		slot.button->m_Image = item.type == 0 || item.count == 0 ? nullptr : m_ItemIcons[item.type - 1];
		slot.button->m_Position.y = 3;

		slot.text->SetText(item.type == 0 || item.count <= 1 || !isStackable[item.type] ? " " : std::to_string(item.count));
		slot.text->m_Position.y = 3;

		slot.button->m_SortingOrder = i != 16 ? 0.6f : 0.7f;
		slot.text->m_SortingOrder = i != 16 ? 0.65f : 0.75f;
	}

	m_InventorySelection->m_Position = m_ItemSlots[m_Selected].button->m_Position + glm::ivec2(-4, -3);
	m_ItemSlots[m_Selected].button->m_Position.y = 4;
	m_ItemSlots[m_Selected].text->m_Position.y = 4;

	m_UseItem = &m_Items[m_Selected];
	if (m_HeldItem.count > 0) m_UseItem = &m_HeldItem;
}

void Inventory::Update(float deltaTime)
{
	if (Input::MouseButtonDown(MOUSE_BUTTON_LEFT))
	{
		for (unsigned int i = 0; i < 16; i++)
		{
			if (m_ItemSlots[i].button->Hover())
			{
				if (m_Items[i].type == m_HeldItem.type && isStackable[m_Items[i].type] && m_HeldItem.count > 0)
				{
					m_Items[i].count += m_HeldItem.count;
					m_HeldItem.count = 0;
				}
				else
				{
					Item itemA = { m_Items[i] };
					Item itemB = { m_HeldItem };
					m_Items[i] = itemB;
					m_HeldItem = itemA;
				}

				UpdateSlots();
			}
		}
	}

	if (Input::MouseButtonHeld(MOUSE_BUTTON_RIGHT))
	{
		for (unsigned int i = 0; i < 16; i++)
		{
			if (m_ItemSlots[i].button->Hover() && (m_GrabTime <= 0.0f || m_FirstGrab))
			{
				if (m_HeldItem.count == 0)
				{
					m_HeldItem.type = m_Items[i].type;
				}

				if (m_Items[i].type == m_HeldItem.type && m_Items[i].count > 0 && isStackable[m_Items[i].type])
				{
					m_HeldItem.count++;
					m_Items[i].count--;

					UpdateSlots();
				}

				if (!m_FirstGrab) m_GrabTime = m_GrabDelay;
				m_FirstGrab = false;
			}
		}
	}
	else
	{
		m_FirstGrab = true;
		m_GrabTime = m_FirstGrabDelay;
	}

	m_GrabTime -= deltaTime;

	if (Input::MouseButtonDown(MOUSE_BUTTON_RIGHT))
	{
		if (!m_InventoryButton->Hover() && m_HeldItem.count > 0 && !Crafting::m_Instance->m_HoveringFrames)
		{
			SpriteManager::m_Player->DropItem(m_HeldItem.type, m_HeldItem.count);
			m_HeldItem.count = 0;

			UpdateSlots();
		}
	}

	if (Input::ScrollUp())
	{
		m_Selected = (m_Selected - 1) % 16;
		UpdateSlots();
	}
	if (Input::ScrollDown())
	{
		m_Selected = (m_Selected + 1) % 16;
		UpdateSlots();
	}

	glm::vec2 pos = Input::MousePosition() / (float)Renderer::pixelSize;
	pos.y = referenceHeight - pos.y;
	pos += glm::vec2(-2, -20);
	m_HeldItemSlot.button->m_Position = pos;
	m_HeldItemSlot.text->m_Position = pos;
}
