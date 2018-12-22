#include "stdafx.h"
#include "PhysicalScene.h"
#include "constants.h"
#include "GameObj.h"


float ConvertPxlsToMeters(float pixels)	{ return pixels / pxlsPerMeter; }
float ConvertMetersToPxls(float meters)	{ return meters * pxlsPerMeter; }

b2Vec2 ConvertPxlsToMeters(const glm::vec2 &pixels) { return b2Vec2(pixels.x / pxlsPerMeter, pixels.y / pxlsPerMeter); }
glm::vec2 ConvertMetersToPxls(const b2Vec2 &meters) { return glm::vec2(meters.x * pxlsPerMeter, meters.y * pxlsPerMeter); }

b2Vec2 ConvertPxlsToMeters(float x, float y)	{ return b2Vec2(x / pxlsPerMeter, y / pxlsPerMeter); }
glm::vec2 ConvertMetersToPxls(float x, float y)	{ return glm::vec2(x * pxlsPerMeter, y * pxlsPerMeter); }

PhysicalScene::PhysicalScene()
	: m_b2World(nullptr)
{
	
}

PhysicalScene::~PhysicalScene()
{
	m_objects.clear();
	m_garbageCollector.clear();
	delete m_b2World;
}

bool PhysicalScene::initialize()
{
	m_b2World = new b2World(b2Vec2(0.0f, 0.0f));
	m_b2World->SetContactListener(this);

	return true;
}

bool PhysicalScene::update(float dt)
{
	m_garbageCollector.clear();

	std::vector<PhysicalObjPtr> temp = m_objects;
	for (PhysicalObjPtr object : temp)
		object->physicalUpdate(dt);

	m_b2World->Step(dt, 6, 8);

	return true;
}

void PhysicalScene::createBounds(float width, float height)
{
	b2Body* walls = nullptr;
	b2BodyDef bd;
	walls = getWorld()->CreateBody(&bd);

	b2EdgeShape shape;
	b2Vec2 levelSize_meters = ConvertPxlsToMeters(glm::vec2(width * BLOCK_SIZE, height * BLOCK_SIZE));

	shape.Set(b2Vec2(0.0f, 0.0f), b2Vec2(levelSize_meters.x, 0.0f));
	b2Fixture* bottomWall = walls->CreateFixture(&shape, 0.0f);
	bottomWall->SetFriction(1.0f);

	shape.Set(b2Vec2(0.0f, 0.0f), b2Vec2(0.0f, levelSize_meters.y));
	b2Fixture* leftWall = walls->CreateFixture(&shape, 0.0f);
	leftWall->SetFriction(1.0f);

	shape.Set(b2Vec2(0.0f, levelSize_meters.y), b2Vec2(levelSize_meters.x, levelSize_meters.y));
	b2Fixture* topWall = walls->CreateFixture(&shape, 0.0f);
	topWall->SetFriction(1.0f);

	shape.Set(b2Vec2(levelSize_meters.x, 0.0f), b2Vec2(levelSize_meters.x, levelSize_meters.y));
	b2Fixture* rightWall = walls->CreateFixture(&shape, 0.0f);
	rightWall->SetFriction(1.0f);
}

void PhysicalScene::addObject(const PhysicalObjPtr& object)
{
	m_objects.push_back(object);
}

void PhysicalScene::removeObject(const PhysicalObjPtr& object)
{
	removeObject(object.get());
}

void PhysicalScene::removeObject(PhysicalObj* object)
{
	for (std::vector<PhysicalObjPtr>::iterator	it =	m_objects.begin(),
														end = m_objects.end();
														it != end;
														++it)
	{
		if ((*it).get() == object)
		{
			m_garbageCollector.push_back(*it);
			m_objects.erase(it);
			return;
		}
	}
}

void PhysicalScene::BeginContact(b2Contact* contact)
{
	b2Fixture* fixtureA = contact->GetFixtureA();
	b2Fixture* fixtureB = contact->GetFixtureB();

	b2Body* bodyA = fixtureA->GetBody();
	b2Body* bodyB = fixtureB->GetBody();

	GameObj* gameObjA = static_cast<GameObj*>(bodyA->GetUserData());
	GameObj* gameObjB = static_cast<GameObj*>(bodyB->GetUserData());

	bool isFixtureASensor = fixtureA->IsSensor();
	bool isFixtureBSensor = fixtureB->IsSensor();

	glm::vec2 contactPower = ConvertMetersToPxls(bodyB->GetLinearVelocity() - bodyA->GetLinearVelocity());

	if (gameObjA)
		gameObjA->processBeginContact(gameObjB, contactPower);

	if (gameObjB)
		gameObjB->processBeginContact(gameObjA, contactPower);
}

void PhysicalScene::PreSolve(b2Contact* contact, const b2Manifold* oldManifold)
{
	/*b2Fixture* fixtureA = contact->GetFixtureA();
	b2Fixture* fixtureB = contact->GetFixtureB();

	b2Body* bodyA = fixtureA->GetBody();
	b2Body* bodyB = fixtureB->GetBody();

	GameObj* gameObjA = static_cast<GameObj*>(bodyA->GetUserData());
	GameObj* gameObjB = static_cast<GameObj*>(bodyB->GetUserData());

	bool isFixtureASensor = fixtureA->IsSensor();
	bool isFixtureBSensor = fixtureB->IsSensor();

	glm::vec2 contactPower = ConvertMetersToPxls(bodyB->GetLinearVelocity() - bodyA->GetLinearVelocity());

	if (gameObjA)
		gameObjA->processBeginContact(gameObjB, contactPower);

	if (gameObjB)
		gameObjB->processBeginContact(gameObjA, contactPower);*/
}

void PhysicalScene::PostSolve(b2Contact* contact, const b2ContactImpulse* impulse)
{

}

void PhysicalScene::EndContact(b2Contact* contact)
{

}