#pragma once

#include "Creature.h"
#include "BlockCursor.h"
#include "boat_parts/BoatPart.h"

#include "../Input.h"

class Player : public Creature
{
private:
	glm::vec3 m_Movement;

	bool m_Climbing;
	bool m_ClimbBlocked;

	bool m_Interacting;

	float m_WalkTime;
	bool m_WalkAnim;

	glm::vec2 m_AimDirection;

	Sprite* m_Cutlass;
	std::vector<Creature*> m_HitCreatures;
	float m_SlashTime;
	const float m_CutlassDuration = 0.3f;
	int m_SlashDirection = 1;

	Sprite* m_Flintlock;
	float m_ShootTime;
	const float m_FlintlockDuration = 0.4f;

	float m_MoveSpeed = 4.5f;
	float m_PlacementRange = 5.0f;

	glm::vec3 m_BeardColor;
	glm::vec3 m_CoatColor;
	glm::vec3 m_HatColor;

	BlockCursor* m_BlockCursor;


public:
	BoatPart* m_InteractTarget;
	BoatPart* m_LastInteractTarget;

public:
	Player();
	~Player();

	void Update(float deltaTime) override;
	void Draw() override;

	void DropItem(unsigned char type, unsigned int count);

	void OnCollision(Body* body) override;
	void OnCollision(BlockGroup* blockGroup, glm::ivec3 blockPos, BlockCollisions side) override;

	void Die() override;

	void Respawn();

	void Remove() override;

	SpriteTypes GetType() override { return SpriteTypes::Player; }

	void SetDescription(std::vector<uint8_t>& desc) override;
	std::vector<uint8_t> GetDescription() const override;
};
