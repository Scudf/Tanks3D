#pragma once

#include "Tank.h"


void NormalizeAngle(float& angle);
float AnglesDifference(float angle0, float angle1);

using TankBotPtr = std::shared_ptr<class TankBot>;

class TankBot :
	public Tank
{
public:
	TankBot(const glm::vec3 &position);
	~TankBot();

	virtual GameObjType getGameObjType() const override { return GameObjType::TankBot; }
	
	static TankBotPtr Create(const glm::vec3 &position) { return std::make_shared<TankBot>(position); }

	virtual void update(float dt);

private:
	float m_timer;
	float m_requiredAngle;
};

