#pragma once

#include "Body.h"

#include "../Input.h"

class Client;

class Player : public Body
{
private:
	Client* m_Client;

	float m_MoveSpeed;

public:
	Player();
	Player(Client* client);
	~Player();

	void Update(float deltaTime) override;
	void Draw() override;

	virtual SpriteTypes GetType() override { return SpriteTypes::Player; }

	virtual void SetDescription(std::vector<uint8_t>& desc) override;
	virtual std::vector<uint8_t> GetDescription() const override;
};
