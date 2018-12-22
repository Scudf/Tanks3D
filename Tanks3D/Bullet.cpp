#include "stdafx.h"
#include "Bullet.h"
#include "Game.h"
#include "GameScene.h"


Bullet::Bullet(const glm::vec3 &position, float angle)
	: GameObj(
		Game::GetInstancePtr()->getGraphicScene()->getModel("normBullet"),
		Game::GetInstancePtr()->getGraphicScene()->getGLShader("normalsShader"),
		position,
		glm::vec2(10.0f, 15.0f),
		false)
{
	getB2Body()->SetTransform(getB2Body()->GetPosition(), angle);
	getB2Body()->GetFixtureList()->SetDensity(10.0f);
	getB2Body()->ResetMassData();
}

void Bullet::processBeginContact(GameObj* gameObj, glm::vec2 contactPower)
{
	Game::GetInstancePtr()->getGameScene()->removeObject(this);
	if (gameObj)
		gameObj->doDamage(10.0f);
}