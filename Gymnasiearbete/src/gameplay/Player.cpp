#include "Player.h"

#include "../graphics/Colors.h"

#include "../ui/Inventory.h"
#include "../ui/Crafting.h"

#include "boat_parts/Cannon.h"
#include "SpriteManager.h"
#include "Raycast.h"
#include "Collision.h"
#include "World.h"

Player::Player()
{
	m_Model = new Model("res/models/sailor.obj", "res/textures/sailor.png", "res/shaders/sailor.shader");

	float offsetRand = randf();
	m_BeardColor = Colors::HSVtoRGB(randf() * 45, 50, randf() * 75);
	m_CoatColor = Colors::HSVtoRGB(randf() * 360, 30, 30 + randf() * 10);
	m_HatColor = Colors::HSVtoRGB(randf() * 35, 30, 30 + randf() * 30);

	m_ColliderRadius = 0.4f;
	m_KnockBackDrag = 5.0f;
}
Player::~Player()
{
	m_Cutlass->Remove();
}

void Player::Update(float deltaTime)
{
	if (m_OwnedHere)
	{
		// Interacting

		bool pressedSpace = false;
		if (m_Interacting)
		{
			if ((Input::KeyDown(KEY_SPACE) && !pressedSpace) || m_LastInteractTarget != m_InteractTarget)
			{
				m_Interacting = false;
				pressedSpace = true;

				if (m_LastInteractTarget != nullptr)
				{
					m_LastInteractTarget->m_Occupied = false;
					m_LastInteractTarget->m_OccupiedHere = false;
					SpriteManager::ForceUpdate(m_LastInteractTarget->m_Id);
				}
			}
		}

		if (m_InteractTarget != nullptr)
		{
			if (Input::KeyDown(KEY_SPACE) && !pressedSpace)
			{
				if (dynamic_cast<Cannon*>(m_InteractTarget) != nullptr)
				{
					dynamic_cast<Cannon*>(m_InteractTarget)->Fire();
				}
				else
				{
					m_Interacting = true;
					m_InteractTarget->m_Occupied = true;
					m_InteractTarget->m_OccupiedHere = true;
				}

				SpriteManager::ForceUpdate(m_InteractTarget->m_Id);

				glm::vec3 dif = m_InteractTarget->m_Position - m_Position;
				TurnSmoothly(-std::atan2(dif.z, dif.x));
			}
			if (m_Interacting)
			{
				m_InteractTarget->Interact(deltaTime);
			}
		}

		m_LastInteractTarget = m_InteractTarget;

		m_InteractTarget = nullptr;

		if (m_Cutlass != nullptr) m_Cutlass->m_Active = false;
		if (m_Flintlock != nullptr) m_Flintlock->m_Active = false;

		if (!m_Interacting)
		{
			glm::vec2 mousePos = Input::MousePosition() / (float)Renderer::pixelSize;
			mousePos -= glm::vec2(referenceWidth, referenceHeight) / 2.0f;

			m_AimDirection = glm::normalize(glm::vec2(mousePos.x * (float)referenceHeight, mousePos.y * (float)referenceWidth / 1.2f));

			m_AimDirection = glm::vec2(
				m_AimDirection.x * glm::cos(-Camera::m_ViewAngle) - m_AimDirection.y * glm::sin(-Camera::m_ViewAngle),
				m_AimDirection.x * glm::sin(-Camera::m_ViewAngle) + m_AimDirection.y * glm::cos(-Camera::m_ViewAngle)
			);

			// Cursor

			if (m_BlockCursor == nullptr)
			{
				m_BlockCursor = new BlockCursor(this);
				SpriteManager::AddSpriteLocally(m_BlockCursor);
			}

			RayHit hit = Camera::RayFromScreen(Input::MousePosition());

			m_BlockCursor->m_Visable = false;
			if (hit.blockGroup != nullptr)
			{
				m_BlockCursor->SetTransform(hit);
				if (glm::distance(m_Position, glm::vec3(m_BlockCursor->m_Position)) <= m_PlacementRange)
				{
					m_BlockCursor->m_Visable = true;
				}
			}
			if (m_BlockCursor->m_Selected.blockGroup != nullptr)
			{
				if (glm::distance(m_Position, glm::vec3(m_BlockCursor->m_Position)) <= m_PlacementRange)
				{
					m_BlockCursor->m_Visable = true;
				}
			}

			// Walking

			m_Movement = glm::vec3(Input::Horizontal(), 0.0f, Input::Vertical());

			if (glm::length(m_Movement) > 1.0f) m_Movement = glm::normalize(m_Movement);

			float angle = Camera::m_ViewAngle;

			glm::mat4 rotationMat(1.0f);
			rotationMat = glm::rotate(rotationMat, angle, glm::vec3(0.0f, 1.0f, 0.0f));

			m_Movement = glm::vec3(rotationMat * glm::vec4(m_Movement, 1.0));

			m_Velocity.x = m_Movement.x * m_MoveSpeed * float(m_Grounded ? 1.0f : 0.5f);
			m_Velocity.z = m_Movement.z * m_MoveSpeed * float(m_Grounded ? 1.0f : 0.5f);

			bool holdingWeapon = Inventory::m_Instance->m_UseItem->count > 0 && (Inventory::m_Instance->m_UseItem->type == CUTLASS || Inventory::m_Instance->m_UseItem->type == FLINTLOCK);
			if (glm::length(m_Movement) > 0.0f && !holdingWeapon)
			{
				TurnSmoothly(-glm::atan(m_Movement.z / m_Movement.x));
				if (m_Movement.x < 0.0f) m_TargetRotation += glm::pi<float>();
			}
			else if (holdingWeapon)
			{
				if (m_SlashTime <= 0.0f)
				{
					TurnSmoothly(-glm::atan(m_AimDirection.y / m_AimDirection.x));
					if (m_AimDirection.x < 0.0f) m_TargetRotation += glm::pi<float>();
				}
				else
				{
					m_TargetRotation = m_Rotation.y;
				}
			}

			// Climbing

			if (m_Climbing && !m_ClimbBlocked) m_Velocity.y = 5.0f;
			m_Climbing = false;
			m_ClimbBlocked = false;
		}
		else
		{
			m_BlockCursor->m_Visable = false;

			m_Movement = glm::vec3(0.0f);

			m_Velocity.x = 0.0f;
			m_Velocity.z = 0.0f;
		}

		// Drowning
		if (m_Position.y <= -1.5f)
		{
			if (m_Position.x < -(worldWidth / 2.0f) - 7.5f ||
				m_Position.x > (worldWidth / 2.0f) + 9.5f ||
				m_Position.z < -(worldHeight / 2.0f) - 7.5f ||
				m_Position.z > (worldHeight / 2.0f) + 9.5f)
			{
				m_Health -= deltaTime * 11.0f;
				m_FlashTime = 1.0f;
			}
		}
	}

	if (glm::length(m_Movement) > 0.0f && m_Grounded)
	{
		m_WalkAnim = true;
	}
	else
	{
		if (glm::mod(m_WalkTime, glm::pi<float>() / 13.0f) > glm::mod(m_WalkTime + deltaTime, glm::pi<float>() / 13.0f)) m_WalkAnim = false;
	}
	if (m_WalkAnim) m_WalkTime += deltaTime;

	// Weapons

	if (m_Cutlass == nullptr)
	{
		m_Cutlass = new Sprite(new Model("res/models/cutlass.obj", "res/textures/cutlass.png", "res/shaders/detailed.shader"));
		m_Cutlass->m_Active = false;
		m_Cutlass->m_Scale = glm::vec3(0.5f);
		SpriteManager::AddSpriteLocally(m_Cutlass);
	}

	if (m_Flintlock == nullptr)
	{
		m_Flintlock = new Sprite(new Model("res/models/flintlock.obj", "res/textures/flintlock.png", "res/shaders/detailed.shader"));
		m_Flintlock->m_Active = false;
		m_Flintlock->m_Scale = glm::vec3(0.6f);
		SpriteManager::AddSpriteLocally(m_Flintlock);
	}

	if (m_OwnedHere)
	{
		if (!m_Interacting && Inventory::m_Instance->m_UseItem->type == CUTLASS && Inventory::m_Instance->m_UseItem->count > 0)
		{
			m_Cutlass->m_Active = true;

			if (Input::MouseButtonDown(MOUSE_BUTTON_LEFT) && m_SlashTime <= -0.25f)
			{
				if (!Inventory::m_Instance->m_InventoryButton->Hover() && !Crafting::m_Instance->m_HoveringFrames)
				{
					m_SlashTime = m_CutlassDuration;

					m_SlashDirection *= -1;
				}
			}
		}
		m_SlashTime -= deltaTime;

		if (!m_Interacting && Inventory::m_Instance->m_UseItem->type == FLINTLOCK && Inventory::m_Instance->m_UseItem->count > 0)
		{
			m_Flintlock->m_Active = true;

			if (Input::MouseButtonDown(MOUSE_BUTTON_LEFT) && m_ShootTime <= -1.50f && !Inventory::m_Instance->m_InventoryButton->Hover() && !Crafting::m_Instance->m_HoveringFrames)
			{
				if (Inventory::m_Instance->Spend(BULLET, 1))
				{
					m_ShootTime = m_FlintlockDuration;

					Bullet* newBullet = new Bullet();

					glm::vec3 offset(
						1.5f * glm::cos(-m_Rotation.y),
						0.5f,
						1.5f * glm::sin(-m_Rotation.y)
					);
					newBullet->m_Position = m_Position + offset;
					newBullet->m_PotentialPosition = m_Position + offset;
					newBullet->m_Rotation = m_Rotation + (glm::pi<float>() / 2.0f);

					SpriteManager::AddSprite(newBullet);
				}
			}
		}
		m_ShootTime -= deltaTime;
	}

	if (m_Cutlass->m_Active)
	{
		if (m_SlashTime <= 0.0f)
		{
			m_HitCreatures.clear();
		}
		else
		{
			glm::vec3 offset(0.5f, 0.3f, 0.5f);
			float swingRot = (glm::pi<float>() * (m_SlashDirection == 1 ? m_SlashTime / m_CutlassDuration : 1.0f - m_SlashTime / m_CutlassDuration)) + 3.8f;
			offset = glm::vec3(
				offset.x * glm::cos(-m_Rotation.y + swingRot) - offset.z * glm::sin(-m_Rotation.y + swingRot),
				offset.y + glm::abs(glm::sin(m_WalkTime * 13.0f) * 0.15f),
				offset.x * glm::sin(-m_Rotation.y + swingRot) + offset.z * glm::cos(-m_Rotation.y + swingRot)
			);

			glm::vec2 hitPoint(m_Position.x + (offset.x * 2.2f), m_Position.z + (offset.z * 2.2f));
			Creature* hitCreature = Collision::SwordOverlap(hitPoint, 0.6f);
			if (hitCreature != nullptr && hitCreature != this)
			{
				if (!std::count(m_HitCreatures.begin(), m_HitCreatures.end(), hitCreature))
				{
					m_HitCreatures.push_back(hitCreature);
					hitCreature->GetHit(20, offset * 8.0f);
				}
			}
		}
	}

	Creature::Update(deltaTime);
}

void Player::Draw()
{
	if (m_Cutlass != nullptr)
	{
		if (m_Cutlass->m_Active)
		{
			if (m_SlashTime <= 0.0f)
			{
				glm::vec3 offset(0.5f, 0.15f, 0.5f);
				offset = glm::vec3(
					offset.x * glm::cos(-m_Rotation.y) - offset.z * glm::sin(-m_Rotation.y),
					offset.y + glm::abs(glm::sin(m_WalkTime * 13.0f) * 0.15f),
					offset.x * glm::sin(-m_Rotation.y) + offset.z * glm::cos(-m_Rotation.y)
				);
				m_Cutlass->m_Position = m_Position + offset;
				m_Cutlass->m_Rotation = m_Rotation + glm::vec3(-0.5f, -0.5f, -0.2f);
			}
			else
			{
				glm::vec3 offset(0.5f, 0.3f, 0.5f);
				float swingRot = (glm::pi<float>() * (m_SlashDirection == 1 ? m_SlashTime / m_CutlassDuration : 1.0f - m_SlashTime / m_CutlassDuration)) + 3.8f;
				offset = glm::vec3(
					offset.x * glm::cos(-m_Rotation.y + swingRot) - offset.z * glm::sin(-m_Rotation.y + swingRot),
					offset.y + glm::abs(glm::sin(m_WalkTime * 13.0f) * 0.15f),
					offset.x * glm::sin(-m_Rotation.y + swingRot) + offset.z * glm::cos(-m_Rotation.y + swingRot)
				);
				m_Cutlass->m_Position = m_Position + offset;
				m_Cutlass->m_Rotation = m_Rotation + glm::vec3(glm::pi<float>() * (m_SlashDirection == 1), -1.0f - swingRot, -0.15f + 0.7f * (m_SlashDirection == 1 ? 1.0f - (m_SlashTime / m_CutlassDuration) : (m_SlashTime / m_CutlassDuration)) - 1.8f);
			}
		}
	}

	if (m_Flintlock != nullptr)
	{
		if (m_Flintlock->m_Active)
		{
			if (m_ShootTime <= 0.0f)
			{
				glm::vec3 offset(0.5f, 0.15f, 0.5f);
				offset = glm::vec3(
					offset.x * glm::cos(-m_Rotation.y) - offset.z * glm::sin(-m_Rotation.y),
					offset.y + glm::abs(glm::sin(m_WalkTime * 13.0f) * 0.15f),
					offset.x * glm::sin(-m_Rotation.y) + offset.z * glm::cos(-m_Rotation.y)
				);
				m_Flintlock->m_Position = m_Position + offset;
				m_Flintlock->m_Rotation = m_Rotation + glm::vec3(0.0f, 0.5f, 1.0f);
			}
			else
			{
				glm::vec3 offset(0.8f, 0.4f, 0.0f);
				offset = glm::vec3(
					offset.x * glm::cos(-m_Rotation.y) - offset.z * glm::sin(-m_Rotation.y),
					offset.y + glm::abs(glm::sin(m_WalkTime * 13.0f) * 0.15f),
					offset.x * glm::sin(-m_Rotation.y) + offset.z * glm::cos(-m_Rotation.y)
				);
				m_Flintlock->m_Position = m_Position + offset;
				m_Flintlock->m_Rotation = m_Rotation + glm::vec3(0.0f, 0.0f, 0.0f);
			}
		}
	}

	if (m_Model == nullptr) return;

	m_Model->m_Shader.Bind();

	m_Model->m_Shader.SetUniform4f("u_BeardColor", m_BeardColor.r, m_BeardColor.g, m_BeardColor.b, 1.0f);
	m_Model->m_Shader.SetUniform4f("u_CoatColor", m_CoatColor.r, m_CoatColor.g, m_CoatColor.b, 1.0f);
	m_Model->m_Shader.SetUniform4f("u_HatColor", m_HatColor.r, m_HatColor.g, m_HatColor.b, 1.0f);

	m_Model->Draw(m_Position + glm::vec3(0.0f, glm::abs(glm::sin(m_WalkTime * 13.0f) * 0.15f), 0.0f), m_Rotation, m_Scale, m_Color, m_Highlighted);
}

void Player::DropItem(unsigned char type, unsigned int count)
{
	DroppedItem* droppedItem = new DroppedItem(type, count);
	droppedItem->m_CanBePickedUp = false;
	droppedItem->m_Position = m_Position + glm::vec3(m_AimDirection.x, 0.5f, m_AimDirection.y) * 1.0f;
	droppedItem->m_KnockBackVelocity = glm::vec3(m_AimDirection.x, 0.0f, m_AimDirection.y) * 3.0f;
	droppedItem->m_Velocity.y = 4.0f;
	SpriteManager::AddSprite(droppedItem);
}

void Player::Die()
{
	Inventory::m_Instance->DropAll(m_Position);

	m_Active = false;
	m_Cutlass->m_Active = false;
	m_Flintlock->m_Active = false;
}

void Player::Respawn()
{
	m_Health = m_MaxHealth;
	m_Dead = false;
	m_Active = true;
}

void Player::Remove()
{
	m_Cutlass->Remove();
	m_Flintlock->Remove();
	Creature::Remove();
}

void Player::OnCollision(Body* body)
{
	if (dynamic_cast<DroppedItem*>(body) != nullptr && m_OwnedHere)
	{
		DroppedItem* item = dynamic_cast<DroppedItem*>(body);
		if (item->m_CanBePickedUp)
		{
			if (Inventory::m_Instance->PickUp(item->m_Type, item->m_Count))
			{
				body->Remove();
				SpriteManager::ForceUpdate(body->m_Id);
			}
		}
	}
}

void Player::OnCollision(BlockGroup* blockGroup, glm::ivec3 blockPos, BlockCollisions side)
{
	if (side == BlockCollisions::Side)
	{
		if (glm::length(m_Movement) > 0.0f)
		{
			m_Climbing = true;
		}
	}
	if (side == BlockCollisions::BlockAbove)
	{
		m_ClimbBlocked = true;
	}

	Body::OnCollision(blockGroup, blockPos, side);
}

void Player::SetDescription(std::vector<uint8_t>& desc)
{
	desc >> m_WillBeRemoved >> m_HatColor >> m_CoatColor >> m_BeardColor >> m_Movement >> m_FlashTime >> m_KnockBackVelocity >> m_Velocity >> m_Active >> m_Scale >> m_Rotation >> m_Position;
	bool cutlassActive;
	bool flintlockActive;
	desc >> m_SlashDirection >> m_SlashTime >> cutlassActive;
	desc >> m_ShootTime >> flintlockActive;
	if (m_Cutlass != nullptr) m_Cutlass->m_Active = cutlassActive;
	if (m_Flintlock != nullptr) m_Flintlock->m_Active = flintlockActive;
}
std::vector<uint8_t> Player::GetDescription() const
{
	std::vector<uint8_t> desc;
	desc << (m_Flintlock != nullptr ? m_Flintlock->m_Active : false) << m_ShootTime;
	desc << (m_Cutlass != nullptr ? m_Cutlass->m_Active : false) << m_SlashTime << m_SlashDirection;
	desc << m_Position << m_Rotation << m_Scale << m_Active << m_Velocity << m_KnockBackVelocity << m_FlashTime << m_Movement << m_BeardColor << m_CoatColor << m_HatColor << m_WillBeRemoved;
	return desc;
}
