#pragma once

#include "PhysicalObj.h"


enum class GameObjType
{
	GameObj,
	Tank,
	TankPlayer,
	TankBot
};

using GameObjPtr = std::shared_ptr<class GameObj>;

class GameObj :
	public PhysicalObj
{
public:
	GameObj(
		const ModelPtr& model,
		const ShaderPtr& shader,
		const glm::vec3 &position,
		const glm::vec2 &size,
		bool isStatic = false);

	virtual ~GameObj();

	virtual GameObjType getGameObjType() const { return GameObjType::GameObj; }

	static GameObjPtr Create(
		const ModelPtr& model,
		const ShaderPtr& shader,
		const glm::vec3 &position,
		const glm::vec2 &size,
		bool isStatic = false) { return std::make_shared<GameObj>(model, shader, position, size, isStatic); }

	virtual void update(float dt);

	virtual void processBeginContact(GameObj* gameObj, glm::vec2 contactPower);
	virtual void processEndContact(GameObj* gameObj);

	void doDamage(float dmg);
	virtual void startDmgEffect();
	virtual void stopDmgEffect();

	float getHealth() { return m_health; }
	void setHealth(float health) { m_health = health; }

protected:
	float m_health;

	float m_dmgTime;
};

