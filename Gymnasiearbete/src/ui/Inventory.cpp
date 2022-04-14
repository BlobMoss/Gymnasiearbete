#include "Inventory.h"

Image* Inventory::m_InventoryBackground;
Image* Inventory::m_ItemIcons[16];

glm::ivec2 Inventory::m_IconPositions[16];

Item Inventory::m_Items[16];
Item Inventory::m_PickedUpItem;

bool Inventory::m_Initialized = false;

void Inventory::Initialize()
{
	m_InventoryBackground = new Image("res/images/inventory.png");
	m_InventoryBackground->m_SortingOrder = 0.8f;

	GenerateIconMeshes();

	m_Initialized = true;
}

void Inventory::GenerateIconMeshes()
{

}

void Inventory::Update(float deltaTime)
{
	if (!m_Initialized) return;
}
void Inventory::Draw()
{
	if (!m_Initialized) return;

	m_InventoryBackground->Draw(glm::ivec2(0, 0));
}