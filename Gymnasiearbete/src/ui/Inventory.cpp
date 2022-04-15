#include "Inventory.h"

#include "../Input.h"

Item Inventory::m_Items[16];
Item Inventory::m_HeldItem;

Inventory::Inventory()
{
	m_InventoryBackground = new UISprite(new Image("res/images/inventory.png"));
	m_InventoryBackground->m_SortingOrder = 0.55f;
	UISpriteManager::AddSprite(m_InventoryBackground);

	for (unsigned int i = 0; i < 32; i++)
	{
		m_ItemIcons[i] = new Image("res/images/item_icons.png", i * 20, (i >= 16) * 20, 20, 20);
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
		m_Items[i] = { 0, 0, true };
	}
	m_HeldItem = { 0, 0, true };

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
		ItemSlot slot = i != 16 ? m_ItemSlots[i] : m_HeldItemSlot;
		Item item = i != 16 ? m_Items[i] : m_HeldItem;

		slot.button->m_Image = item.type == 0 || item.count == 0 ? nullptr : m_ItemIcons[item.type - 1];

		slot.text->SetText(item.type == 0 || item.count == 0 || !item.stackable ? " " : std::to_string(item.count));

		slot.button->m_SortingOrder = i != 16 ? 0.6f : 0.7f;
		slot.text->m_SortingOrder = i != 16 ? 0.65f : 0.75f;
	}
}

void Inventory::Update(float deltaTime)
{
	for (unsigned int i = 0; i < 16; i++)
	{
		if (Input::MouseButtonDown)
		if (m_ItemSlots[i].button->Down())
		{
			Item itemA = { m_Items[i] };
			Item itemB = { m_HeldItem };
			m_Items[i] = itemB;
			m_HeldItem = itemA;

			UpdateSlots();
		}
	}
}

void Inventory::Draw()
{
	glm::vec2 pos = Input::MousePosition() / (float)Renderer::pixelSize;
	pos.y = referenceHeight - pos.y;
	pos += glm::vec2(4, -26);
	m_HeldItemSlot.button->m_Position = pos;
	m_HeldItemSlot.text->m_Position = pos;
}