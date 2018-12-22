#pragma once

#include <Box2D/Box2D.h>
#include <functional>
#include <glm/glm.hpp>
#include "GameObj.h"


float RayCast(
	const glm::vec2& from,
	const glm::vec2& to,
	std::function< bool(const GameObj* gameObj)> filter);

class RayCastWrapper : public b2RayCastCallback
{
public:
	using Callback = std::function< float32(
		b2Fixture* fixture,
		const b2Vec2& point,
		const b2Vec2& normal,
		float32 fraction) >;

	RayCastWrapper(Callback callback) :
		m_callback(callback) {}

public:
	virtual float32 ReportFixture(
		b2Fixture* fixture,
		const b2Vec2& point,
		const b2Vec2& normal,
		float32 fraction) override
	{
		return m_callback(fixture, point, normal, fraction);
	}

private:
	Callback m_callback;
};