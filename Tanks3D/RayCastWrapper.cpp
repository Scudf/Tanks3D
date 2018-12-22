#include "stdafx.h"
#include "RayCastWrapper.h"
#include "PhysicalScene.h"
#include "Game.h"


float RayCast(
	const glm::vec2& from,
	const glm::vec2& to,
	std::function< bool(const GameObj* gameObj)> filter)
{
	b2Vec2 position = ConvertPxlsToMeters(from);
	b2Vec2 target = ConvertPxlsToMeters(to);

	// Box2D assert prevention
	if (target == position)
		return 0;

	float result = 0;

	RayCastWrapper callback(
		[&](
			b2Fixture* fixture,
			const b2Vec2& point,
			const b2Vec2& normal,
			float32 fraction) -> float32
	{
		b2Body* body = fixture->GetBody();

		if (!body)
			return 0;

		void* userData = fixture->GetBody()->GetUserData();
		GameObj* gameObj;

		if (userData)
		{
			gameObj = static_cast<GameObj*>(userData);
		}
		else
		{
			gameObj = nullptr;
		}

		if (filter && !filter(gameObj))
		{
			return -1;
		}

		glm::vec2 pt = ConvertMetersToPxls(point);

		result = (pt - from).length();

		return 0;
	});


	b2World* world = Game::GetInstancePtr()->getPhysicalScene()->getWorld();

	world->RayCast(
		&callback,
		position,
		target);

	return result;
}