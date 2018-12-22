#include "stdafx.h"
#include "GraphicObj.h"
#include <glm/gtc/matrix_transform.hpp>
#include "Game.h"
#include "GraphicScene.h"


GraphicObj::GraphicObj(
	const ModelPtr& model,
	const ShaderPtr& shader,
	const glm::vec3 &position,
	const glm::vec3 &rotation,
	const glm::vec3 &scale)
	: m_model(model)
	, m_shader(shader)
	, m_position(position)
	, m_rotation(rotation)
	, m_scale(scale)
	, m_color(1.0f, 1.0f, 1.0f)
	, m_ambientColor(0.5f, 0.5f, 0.5f)
	, m_diffuseColor(0.5f, 0.5f, 0.5f)
	, m_specularColor(0.5f, 0.5f, 0.5f)
	, m_shininess(16.0f)
	, m_saturation(1.0f)
	, m_brightness(1.0f)
	, m_diffuseTexInd(-1)
	, m_specularTexInd(-1)
	, m_matModelIsDirty(true)
	, m_matModel(1)
	, m_colorIsDirty(true)
{	

}

void GraphicObj::render()
{
	m_shader->Use();

	if (m_diffuseTexInd != -1 && m_specularTexInd != -1)
	{
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, m_diffuseTexInd);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, m_specularTexInd);
	}

	GraphicScene* gs = Game::GetInstancePtr()->getGraphicScene();

	if (m_matModelIsDirty)
	{
		m_matModel = glm::mat4(1);
		m_matModel = glm::translate(m_matModel, m_position);
		m_matModel = glm::rotate(m_matModel, m_rotation.x, glm::vec3(1, 0, 0));
		m_matModel = glm::rotate(m_matModel, m_rotation.y, glm::vec3(0, 1, 0));
		m_matModel = glm::rotate(m_matModel, m_rotation.z, glm::vec3(0, 0, 1));
		m_matModel = glm::scale(m_matModel, m_scale);
		m_matModelIsDirty = false;
	}

	m_shader->setUniform("u_matModel", m_matModel);
	m_shader->setUniform("u_matView", gs->getMatView());
	m_shader->setUniform("u_matPersp", gs->getMatPersp());

	if (m_shader->hasUniform("u_viewPosition"))
		m_shader->setUniform("u_viewPosition", gs->getCamera()->getPosition());

	if (gs->getLightIsDirty())
	{
		if (m_shader->hasUniform("u_light.direction"))
			m_shader->setUniform("u_light.direction", gs->getLightDirection());

		if (m_shader->hasUniform("u_light.ambient"))
			m_shader->setUniform("u_light.ambient", gs->getLightAmbient());

		if (m_shader->hasUniform("u_light.diffuse"))
			m_shader->setUniform("u_light.diffuse", gs->getLightDiffuse());

		if (m_shader->hasUniform("u_light.specular"))
			m_shader->setUniform("u_light.specular", gs->getLightSpecular());
	}
		
	if (m_shader->hasUniform("u_color"))
		m_shader->setUniform("u_color", m_color);

	if (m_shader->hasUniform("u_material.shininess"))
		m_shader->setUniform("u_material.shininess", m_shininess);

	if (m_shader->hasUniform("u_material.saturation"))
		m_shader->setUniform("u_material.saturation", m_saturation);

	if (m_shader->hasUniform("u_material.brightness"))
		m_shader->setUniform("u_material.brightness", m_brightness);

	if (m_shader->hasUniform("u_material.ambientColor"))
		m_shader->setUniform("u_material.ambientColor", m_ambientColor);

	if (m_shader->hasUniform("u_material.diffuseColor"))
		m_shader->setUniform("u_material.diffuseColor", m_diffuseColor);

	if (m_shader->hasUniform("u_material.specularColor"))
		m_shader->setUniform("u_material.specularColor", m_specularColor);

	if (m_shader->hasUniform("u_material.diffuseTex"))
		m_shader->setUniform("u_material.diffuseTex", 0);

	if (m_shader->hasUniform("u_material.specularTex"))
		m_shader->setUniform("u_material.specularTex", 1);;

	GLuint lineInd[] = {
		0, 1
	};

	GLuint planeInd[] = {
		0, 1, 2,
		3, 1, 2
	};

	glBindVertexArray(m_model->getVAO());

	switch (m_model->getModelType())
	{
	case ModelType::V2:		glDrawElements(GL_LINES, 2, GL_UNSIGNED_INT, lineInd);		break;
	case ModelType::V4:		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, planeInd);	break;
	case ModelType::V36:	glDrawArrays(GL_TRIANGLES, 0, 36);							break;
	case ModelType::V72:	glDrawArrays(GL_TRIANGLES, 0, 72);							break;
	}
}

void GraphicObj::setMaterialColor(
	const glm::vec3 &ambientColor,
	const glm::vec3 &diffuseColor,
	const glm::vec3 &specularColor,
	float shininess,
	float saturation,
	float brightness)
{
	m_ambientColor = ambientColor;
	m_diffuseColor = diffuseColor;
	m_specularColor = specularColor;
	m_shininess = shininess;
	m_saturation = saturation;
	m_brightness = brightness;
	m_colorIsDirty = true;
}
void GraphicObj::setMaterialTexture(
	int diffuseTexInd,
	int specularTexInd,
	float shininess,
	float saturation,
	float brightness)
{
	m_diffuseTexInd = diffuseTexInd;
	m_specularTexInd = specularTexInd;
	m_shininess = shininess;
	m_saturation = saturation;
	m_brightness = brightness;
	m_colorIsDirty = true;
}