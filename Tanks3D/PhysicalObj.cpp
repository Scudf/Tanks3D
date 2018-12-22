#include "stdafx.h"
#include "PhysicalObj.h"
#include "Game.h"
#include "GraphicScene.h"
#include "PhysicalScene.h"


PhysicalObj::PhysicalObj(
	const ModelPtr& model,
	const ShaderPtr& shader,
	const glm::vec3 &position,
	const glm::vec2 &size,
	bool isStatic)
{
	GraphicScene*  gs = Game::GetInstancePtr()->getGraphicScene();
	PhysicalScene* ps = Game::GetInstancePtr()->getPhysicalScene();

	m_graphicObj = GraphicObj::Create(model, shader);
	gs->addObject(m_graphicObj);

	b2BodyDef bodyDef;
	bodyDef.position.Set(ConvertPxlsToMeters(position.x), ConvertPxlsToMeters(position.y));

	if (isStatic)
		bodyDef.type = b2_staticBody;
	else
		bodyDef.type = b2_dynamicBody;

	m_objBody = ps->getWorld()->CreateBody(&bodyDef);

	b2PolygonShape poligonShape;
	poligonShape.SetAsBox(ConvertPxlsToMeters(size.x) / 2, ConvertPxlsToMeters(size.y) / 2);
	m_z = position.z;

	b2FixtureDef fixtureDef;
	fixtureDef.shape = &poligonShape;
	m_objBody->CreateFixture(&fixtureDef);
}

PhysicalObj::~PhysicalObj()
{
	Game::GetInstancePtr()->getPhysicalScene()->getWorld()->DestroyBody(m_objBody);
	Game::GetInstancePtr()->getGraphicScene()->removeObject(m_graphicObj);
}

void PhysicalObj::physicalUpdate(float dt)
{
	m_graphicObj->setPosition(glm::vec3(ConvertMetersToPxls(m_objBody->GetPosition().x), ConvertMetersToPxls(m_objBody->GetPosition().y), m_z));
	m_graphicObj->setRotation(glm::vec3(m_graphicObj->getRotation().x, m_graphicObj->getRotation().y, m_objBody->GetAngle()));
}