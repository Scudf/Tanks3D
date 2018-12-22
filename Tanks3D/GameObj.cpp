#include "stdafx.h"
#include "GameObj.h"
#include "Game.h"
#include "GameScene.h"


GameObj::GameObj(
	const ModelPtr& model,
	const ShaderPtr& shader,
	const glm::vec3 &position,
	const glm::vec2 &size,
	bool isStatic)
	: PhysicalObj(
		model,
		shader,
		position,
		size,
		isStatic)
	, m_health(50.0f)
	, m_dmgTime(0.0f)
{
	getB2Body()->SetUserData(this);
}

GameObj::~GameObj()
{

}

void GameObj::update(float dt)
{
	if (m_dmgTime > 0)
		m_dmgTime -= dt;
	else
		stopDmgEffect();	
}

void GameObj::processBeginContact(GameObj* gameObj, glm::vec2 contactPower)
{

}

void GameObj::processEndContact(GameObj* gameObj)
{

}

void GameObj::doDamage(float dmg)
{
	startDmgEffect();
	m_health -= dmg;
	if (m_health <= 0)
		Game::GetInstancePtr()->getGameScene()->removeObject(this);
}

void GameObj::startDmgEffect()
{
	m_dmgTime = 0.05f;
	getGraphicObject()->setSaturation(2.0f);
	getGraphicObject()->setBrightness(2.0f);
}

void GameObj::stopDmgEffect()
{
	m_dmgTime = 0;
	getGraphicObject()->setSaturation(1.0f);
	getGraphicObject()->setBrightness(1.0f);
}