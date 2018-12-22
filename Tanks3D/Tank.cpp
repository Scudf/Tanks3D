#include "stdafx.h"
#include "Tank.h"
#include "GraphicScene.h"
#include "PhysicalScene.h"
#include "constants.h"
#include "Game.h"
#include "RayCastWrapper.h"
#include "SoundManager.h"


Tank::Tank(const glm::vec3 &position)
	: GameObj(
		Game::GetInstancePtr()->getGraphicScene()->getModel("texCaterpillar"),
		Game::GetInstancePtr()->getGraphicScene()->getGLShader("texturesShader"),
		position,
		glm::vec2(BLOCK_SIZE * 1.5f, BLOCK_SIZE * 2.0f), false)
	, m_movementForward(false)
	, m_movementLeft(false)
	, m_movementBackward(false)
	, m_movementRight(false)
	, m_clockwiseTowerRotate(false)
	, m_conterclockwiseTowerRotate(false)
	, m_towerAngle(glm::radians(90.0f))
	, m_reloadTime(0.0f)
	, m_isReloaded(true)
	, m_isFireAllowed(false)
{
	GraphicScene*	gs = Game::GetInstancePtr()->getGraphicScene();
	getGraphicObject()->setMaterialTexture(gs->getGLTexture("textures/Caterpillar_Diffuse.png"), gs->getGLTexture("textures/Caterpillar_Specular.png"));
	getB2Body()->SetAngularDamping(2);
	getB2Body()->SetLinearDamping(1);
	getB2Body()->GetFixtureList()->SetDensity(1);
	getB2Body()->ResetMassData();
	
	m_tankTower = GraphicObj::Create(gs->getModel("texTower"), gs->getGLShader("texturesShader"), glm::vec3(position.x, position.y, position.z * 2 + getZ()));
	m_tankTower->setMaterialTexture(gs->getGLTexture("textures/Tower_Diffuse.png"), gs->getGLTexture("textures/Tower_Specular.png"));
	gs->addObject(m_tankTower);

	m_health = 200.0f;
}

Tank::~Tank()
{
	Game::GetInstancePtr()->getGraphicScene()->removeObject(m_tankTower);
}

void Tank::update(float dt)
{
	GameObj::update(dt);
	m_tankTower->setPosition(glm::vec3(ConvertMetersToPxls(getB2Body()->GetPosition()).x, ConvertMetersToPxls(getB2Body()->GetPosition()).y, getZ() + 50));

	if (!m_isReloaded)
		m_reloadTime += dt;

	if (m_reloadTime >= 0.15f)
	{
		m_isReloaded = true;
		m_reloadTime = -0.15f;
	}

	if (m_isReloaded && m_isFireAllowed)
	{
		m_bulletSpawnAngle = m_towerAngle + getB2Body()->GetAngle() - glm::radians(90.0f);
		m_bulletSpawnPosition = glm::vec3(
			ConvertMetersToPxls(getB2Body()->GetPosition().x) - sin(m_bulletSpawnAngle) * 200.0f,
			ConvertMetersToPxls(getB2Body()->GetPosition().y) + cos(m_bulletSpawnAngle) * 200.0f, 75.0f);

		float raycast = RayCast(
			ConvertMetersToPxls(getB2Body()->GetPosition()),
			m_bulletSpawnPosition,
			[this](const GameObj* gameObj) -> bool
			{
				if (this == gameObj)
					return false;
				else
					return true;
			});

		if (raycast == 0)
		{
			Game::GetInstancePtr()->getSoundManager()->playSound3D("sounds/shot.mp3", m_bulletSpawnPosition);
			Game::GetInstancePtr()->getGameScene()->createBulet(m_bulletSpawnPosition, m_bulletSpawnAngle);
			m_isReloaded = false;
		}
	}
}

void Tank::physicalUpdate(float dt)
{
	PhysicalObj::physicalUpdate(dt);
	
	m_tankDir = glm::vec2(-sin(getB2Body()->GetAngle()), cos(getB2Body()->GetAngle()));

	if (m_movementForward)
		getB2Body()->ApplyLinearImpulse(ConvertPxlsToMeters(m_tankDir * 800.0f * dt * getB2Body()->GetMass()), getB2Body()->GetWorldCenter(), true);
	if (m_movementLeft)
		getB2Body()->ApplyAngularImpulse(2.5f * dt * getB2Body()->GetMass(), true);
	if (m_movementBackward)
		getB2Body()->ApplyLinearImpulse(ConvertPxlsToMeters(-m_tankDir * 400.0f * dt * getB2Body()->GetMass()), getB2Body()->GetWorldCenter(), true);
	if (m_movementRight)
		getB2Body()->ApplyAngularImpulse(-2.5f * dt * getB2Body()->GetMass(), true);
	if (m_clockwiseTowerRotate)
		m_towerAngle += 1.84f * dt;
	if (m_conterclockwiseTowerRotate)
		m_towerAngle -= 1.84f * dt;

	m_tankTower->setRotation(glm::vec3(0.0f, 0.0f, m_towerAngle + getB2Body()->GetAngle()));
}

void Tank::startDmgEffect()
{
	GameObj::startDmgEffect();

	m_tankTower->setSaturation(2.0f);
	m_tankTower->setBrightness(2.0f);
}

void Tank::stopDmgEffect()
{
	GameObj::stopDmgEffect();

	m_tankTower->setSaturation(1.0f);
	m_tankTower->setBrightness(1.0f);
}