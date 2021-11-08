#include "SpriteManager.h"

#include "Model.h"

SpriteManager::SpriteManager()
	: m_Gem(LoadOBJ("res/models/gem.obj"), "res/textures/gem_texture.png", "res/shaders/lighting.shader"),
	m_Teapot(LoadOBJ("res/models/teapot.obj"), "res/textures/teapot_texture.png", "res/shaders/lighting.shader")
{
	m_Gem.SetPosition(glm::vec3(-2, 0, -2));
	m_Teapot.SetPosition(glm::vec3(1, -1, 0));
}
SpriteManager::~SpriteManager()
{

}

void SpriteManager::Update(float deltaTime)
{
	m_Gem.Update(deltaTime);
	m_Teapot.Update(deltaTime);
}
void SpriteManager::Draw(Renderer renderer)
{
	m_Gem.Draw(renderer);
	m_Teapot.Draw(renderer);
}