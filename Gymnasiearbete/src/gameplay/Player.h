#pragma once

#include "Body.h"
#include "BlockCursor.h"

#include "../Input.h"

class Player : public Body
{
private:
	glm::vec3 m_Movement;

	bool climbing;
	bool climbBlocked;

	float m_WalkTime;
	bool m_WalkAnim;

	float m_MoveSpeed = 4.5f;
	float m_PlacementRange = 5.0f;

	glm::vec3 m_BeardColor;
	glm::vec3 m_CoatColor;
	glm::vec3 m_HatColor;

	BlockCursor* m_BlockCursor;

public:
	Player();
	~Player();

	virtual void Update(float deltaTime) override;
	virtual void Draw() override;

	virtual void OnCollision(Body* body) override;
	virtual void OnCollision(BlockGroup* blockGroup, BlockCollisions side) override;

	virtual SpriteTypes GetType() override { return SpriteTypes::Player; }

	virtual void SetDescription(std::vector<uint8_t>& desc) override;
	virtual std::vector<uint8_t> GetDescription() const override;
};
