#include "Inventory.h"

#include "../Input.h"

Item Inventory::m_Items[16];
Item Inventory::m_HeldItem;

Inventory::Inventory()
{
	m_InventoryBackground = new UISprite(new Image("res/images/inventory.png"));
	m_InventoryBackground->m_Image->m_SortingOrder = 0.6f;
	UISpriteManager::AddSprite(m_InventoryBackground);

	for (unsigned int i = 0; i < 32; i++)
	{
		m_ItemIcons[i] = new Image("res/images/item_icons.png", i * 20, (i >= 16) * 20, 20, 20);
		m_ItemIcons[i]->m_SortingOrder = 0.8f;
	}

	for (unsigned int i = 0; i < 17; i++)
	{
		UIButton* button = new UIButton();
		button->m_Position = glm::ivec2(115 + (i * 26), 3);
		if (i >= 8) button->m_Position.x--;
		UISpriteManager::AddSprite(button);

		UIText* text = new UIText(" ");
		text->m_Position = button->m_Position;
		text->m_Image->m_SortingOrder = 0.9f;
		UISpriteManager::AddSprite(text);

		if (i < 16) m_ItemSlots[i] = { button, text }; else m_HeldItemSlot = { button, text };
	}

	for (unsigned int i = 0; i < 16; i++)
	{
		m_Items[i] = { 0, 0, true };
	}

	m_HeldItem.type = 1;
	m_HeldItem.count = 4;
	m_HeldItem.stackable = true;

	m_Items[2].type = 1;
	m_Items[2].count = 8;
	m_Items[2].stackable = true;


	UpdateSlots();
}

Inventory::~Inventory()
{

}

void Inventory::UpdateSlots()
{
	for (unsigned int i = 0; i < 17; i++)
	{
		ItemSlot slot = i < 16 ? m_ItemSlots[i] : m_HeldItemSlot;
		Item item = i < 16 ? m_Items[i] : m_HeldItem;

		slot.button->m_Image = item.type == 0 || item.count == 0 ? nullptr : m_ItemIcons[item.type - 1];

		slot.text->SetText(item.type == 0 || item.count == 0 || !item.stackable ? " " : std::to_string(item.count));
	}
}

void Inventory::Update(float deltaTime)
{
	
}

void Inventory::Draw()
{
	glm::vec2 pos = Input::MousePosition() / (float)Renderer::pixelSize;
	pos.y = referenceHeight - pos.y;
	pos += glm::vec2(5, -26);
	m_HeldItemSlot.button->m_Position = pos;
	m_HeldItemSlot.text->m_Position = pos;
}