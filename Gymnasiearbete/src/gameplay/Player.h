#pragma once

#include "Body.h"

#include "../Input.h"

class Client;

class Player : public Body
{
private:
	Client* m_Client;

	float m_WalkTime;
	bool m_WalkAnim;

	float m_MoveSpeed;

	glm::vec3 m_BeardColor;
	glm::vec3 m_CoatColor;
	glm::vec3 m_HatColor;

public:
	Player();
	Player(Client* client);
	~Player();

	virtual void Update(float deltaTime) override;
	virtual void Draw() override;
	virtual void Move(float deltaTime) override;

	virtual SpriteTypes GetType() override { return SpriteTypes::Player; }

	virtual void SetDescription(std::vector<uint8_t>& desc) override;
	virtual std::vector<uint8_t> GetDescription() const override;
};
