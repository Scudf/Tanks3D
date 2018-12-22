#pragma once

#include <Box2D/Box2D.h>
#include <glm/glm.hpp>
#include "PhysicalObj.h"


static const float pxlsPerMeter = 100;

float ConvertPxlsToMeters(float pixels);
float ConvertMetersToPxls(float meters);

b2Vec2 ConvertPxlsToMeters(const glm::vec2 &pixels);
glm::vec2 ConvertMetersToPxls(const b2Vec2 &meters);

b2Vec2 ConvertPxlsToMeters(float x, float y);
glm::vec2 ConvertMetersToPxls(float x, float y);

class PhysicalScene	:
	public b2ContactListener
{
public:
	PhysicalScene();
	~PhysicalScene();

	bool initialize();
	bool update(float dt);

	void createBounds(float width, float height);

	b2World* getWorld() const { return m_b2World; }

	void addObject(const PhysicalObjPtr& object);
	void removeObject(const PhysicalObjPtr& object);
	void removeObject(PhysicalObj* object);

	virtual void BeginContact(b2Contact* contact) override;
	virtual void PreSolve(b2Contact* contact, const b2Manifold* oldManifold) override;
	virtual void PostSolve(b2Contact* contact, const b2ContactImpulse* impulse) override;
	virtual void EndContact(b2Contact* contact) override;

private:
	b2World* m_b2World;

	std::vector<PhysicalObjPtr> m_objects;
	std::vector<PhysicalObjPtr> m_garbageCollector;
};

