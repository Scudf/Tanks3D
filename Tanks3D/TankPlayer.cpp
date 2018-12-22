#include "stdafx.h"
#include "TankPlayer.h"
#include "Game.h"


TankPlayer::TankPlayer(const glm::vec3 &position)
	: Tank(position)
{
	getTankTower()->setColor(glm::vec3(0.0f, 0.0f, 1.0f));
	getGraphicObject()->setColor(glm::vec3(0.0f, 0.0f, 1.0f));
	m_health = 1000.0f;
}

void TankPlayer::update(float dt)
{
	Tank::update(dt);

	GameScene* gs = Game::GetInstancePtr()->getGameScene();

	m_movementForward = gs->getKey(GLFW_KEY_W);
	m_movementLeft = gs->getKey(GLFW_KEY_A);
	m_movementBackward = gs->getKey(GLFW_KEY_S);
	m_movementRight = gs->getKey(GLFW_KEY_D);
	m_clockwiseTowerRotate = gs->getKey(GLFW_KEY_Q);
	m_conterclockwiseTowerRotate = gs->getKey(GLFW_KEY_E);
	m_isFireAllowed = gs->getKey(GLFW_KEY_SPACE);
}