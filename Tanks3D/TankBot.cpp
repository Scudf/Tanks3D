#include "stdafx.h"
#include "TankBot.h"
#include "Game.h"
#include "GameScene.h"
#include "RayCastWrapper.h"
#include "GameObj.h"


void NormalizeAngle(float& angle)
{
	while (angle < -glm::pi<float>()) angle += glm::two_pi<float>();
	while (angle > glm::pi<float>()) angle -= glm::two_pi<float>();
}

float AnglesDifference(float angle0, float angle1)
{
	NormalizeAngle(angle0);
	NormalizeAngle(angle1);

	if (glm::abs(angle1 - angle0) > glm::pi<float>())
	{
		if (angle1 > angle0)
			angle0 += glm::two_pi<float>();
		else
			angle1 += glm::two_pi<float>();
	}

	return angle1 - angle0;
}

TankBot::TankBot(const glm::vec3 &position)
	: Tank(position)
	, m_timer(0)
{
	getTankTower()->setColor(glm::vec3(1.0f, 0.0f, 0.0f));
	getGraphicObject()->setColor(glm::vec3(1.0f, 0.0f, 0.0f));
}

TankBot::~TankBot()
{
	Game::GetInstancePtr()->getGameScene()->setEnemiesCount(Game::GetInstancePtr()->getGameScene()->getEnemiesCount() - 1);
}

void TankBot::update(float dt)
{
	Tank::update(dt);

	glm::vec2 targetPosition = ConvertMetersToPxls(Game::GetInstancePtr()->getGameScene()->getTankPtr()->getB2Body()->GetPosition());
	glm::vec2 distanceToTarget = targetPosition - ConvertMetersToPxls(getB2Body()->GetPosition());

	glm::vec2 v = glm::vec2(glm::normalize(distanceToTarget));
	m_requiredAngle = (v.y >= 0 ? acosf(glm::dot(glm::vec2(1, 0), v))
		: -acosf(glm::dot(glm::vec2(1, 0), v)));

	m_isFireAllowed = false;

	float disToTargLength = sqrt(distanceToTarget.x * distanceToTarget.x + distanceToTarget.y * distanceToTarget.y);

	if (disToTargLength <= 1000.0f)
	{
		float raycast = RayCast(
			ConvertMetersToPxls(getB2Body()->GetPosition()),
			targetPosition,
			[this](const GameObj* gameObj) -> bool
		{
			if (this == gameObj)
				return false;

			if (gameObj)
			{
				if (gameObj->getGameObjType() == GameObjType::TankPlayer || gameObj->getGameObjType() == GameObjType::GameObj)
					return false;
				else
					return true;
			}
			else
			{
				return true;
			}
		});

		if (raycast == 0)
		{
			m_isFireAllowed = true;

			float totalRotation = m_requiredAngle - m_towerAngle - getB2Body()->GetAngle();
			NormalizeAngle(totalRotation);

			if (totalRotation <= glm::pi<float>() && totalRotation > 0.1f)
			{
				m_conterclockwiseTowerRotate = false;
				m_clockwiseTowerRotate = true;
			}

			else if (totalRotation > -glm::half_pi<float>() && totalRotation < -0.1f)
			{
				m_clockwiseTowerRotate = false;
				m_conterclockwiseTowerRotate = true;
			}

			if (totalRotation >= -0.1f && totalRotation <= 0.1f)
			{
				m_clockwiseTowerRotate = false;
				m_conterclockwiseTowerRotate = false;
			}

		}
	}

	if (m_isFireAllowed)
	{
		m_movementLeft = false;
		m_movementRight = false;
	}

	m_timer += dt;

	if (m_timer >= 3.0f)
	{
		m_timer = 0;

		m_movementForward = (bool)(rand() % 2 - 1);
		m_movementBackward = (bool)(rand() % 2 - 1);

		if (!m_isFireAllowed)
		{
			m_movementLeft = (bool)(rand() % 2 - 1);
			m_movementRight = (bool)(rand() % 2 - 1);
			m_clockwiseTowerRotate = (bool)(rand() % 2 - 1);
			m_conterclockwiseTowerRotate = (bool)(rand() % 2 - 1);
		}
	}
}
