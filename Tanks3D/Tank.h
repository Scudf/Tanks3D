#pragma once

#include <glm/glm.hpp>
#include "PhysicalObj.h"
#include "GraphicObj.h"
#include "GameObj.h"
#include "constants.h"


using TankPtr = std::shared_ptr<class Tank>;

class Tank :
	public GameObj
{
public:
	Tank(const glm::vec3 &position);
	~Tank();

	virtual GameObjType getGameObjType() const override { return GameObjType::Tank; }

	static TankPtr Create(const glm::vec3 &position) { return std::make_shared<Tank>(position); }

	virtual void update(float dt);
	void physicalUpdate(float dt);

	void startDmgEffect();
	void stopDmgEffect();

	const GraphicObjPtr& getTankTower() { return m_tankTower; }

protected:
	GraphicObjPtr m_tankTower;

	bool m_movementForward;
	bool m_movementBackward;
	bool m_movementLeft;
	bool m_movementRight;
	bool m_clockwiseTowerRotate;
	bool m_conterclockwiseTowerRotate;

	float m_reload;

	float m_towerAngle;
	glm::vec2 m_tankDir;

	float m_reloadTime;
	bool m_isReloaded;
	bool m_isFireAllowed;

	glm::vec3 m_bulletSpawnPosition;
	float m_bulletSpawnAngle;
};

