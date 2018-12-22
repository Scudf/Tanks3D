#pragma once

#include <Box2D/Box2D.h>
#include "GraphicObj.h"
#include <memory>


using PhysicalObjPtr = std::shared_ptr<class PhysicalObj>;

class PhysicalObj
{
public:
	PhysicalObj(
		const ModelPtr& model,
		const ShaderPtr& shader,
		const glm::vec3 &position, 
		const glm::vec2 &size, 
		bool isStatic = false);

	virtual ~PhysicalObj();

	static PhysicalObjPtr Create(
		const ModelPtr& model,
		const ShaderPtr& shader,
		const glm::vec3 &position,
		const glm::vec2 &size,
		bool isStatic = false) { return std::make_shared<PhysicalObj>(model, shader, position, size, isStatic); }

	virtual void physicalUpdate(float dt);

	b2Body*	getB2Body() { return m_objBody; }
	const GraphicObjPtr& getGraphicObject()	{ return m_graphicObj; }

	float getZ() { return m_z; }

private:
	b2Body* m_objBody;
	GraphicObjPtr m_graphicObj;

	float m_z;
};

