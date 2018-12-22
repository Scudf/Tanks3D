#pragma once

#include "Tank.h"


using TankPlayerPtr = std::shared_ptr<class TankPlayer>;

class TankPlayer :
	public Tank
{
public:
	TankPlayer(const glm::vec3 &position);

	virtual GameObjType getGameObjType() const override { return GameObjType::TankPlayer; }

	static TankPlayerPtr Create(const glm::vec3 &position) { return std::make_shared<TankPlayer>(position); }

	virtual void update(float dt);
};

