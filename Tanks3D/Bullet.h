#pragma once

#include "GameObj.h"


using BulletPtr = std::shared_ptr<class Bullet>;

class Bullet :
	public GameObj
{
public:
	Bullet(const glm::vec3 &position, float angle);

	static BulletPtr Create(const glm::vec3 &position, float angle) { return std::make_shared<Bullet>(position, angle); }

	virtual void processBeginContact(GameObj* gameObj, glm::vec2 contactPower) override;
};

