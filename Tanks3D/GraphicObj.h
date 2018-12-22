#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>
#include "Model.h"
#include "Shader.h"
#include <memory>


using GraphicObjPtr = std::shared_ptr<class GraphicObj>;

class GraphicObj
{
public:
	GraphicObj(
		const ModelPtr& model,
		const ShaderPtr& shader,
		const glm::vec3 &position = glm::vec3(0.0f, 0.0f, 0.0f),
		const glm::vec3 &rotation = glm::vec3(0.0f, 0.0f, 0.0f),
		const glm::vec3 &scale = glm::vec3(1.0f, 1.0f, 1.0f));

	static GraphicObjPtr Create(
		const ModelPtr& model,
		const ShaderPtr& shader,
		const glm::vec3 &position = glm::vec3(0.0f, 0.0f, 0.0f),
		const glm::vec3 &rotation = glm::vec3(0.0f, 0.0f, 0.0f),
		const glm::vec3 &scale = glm::vec3(1.0f, 1.0f, 1.0f)) { return std::make_shared<GraphicObj>(model, shader, position, rotation, scale); }

	void render();

	void setPosition(const glm::vec3 &position) { m_position = position;	m_matModelIsDirty = true; }
	void setRotation(const glm::vec3 &rotation) { m_rotation = rotation;	m_matModelIsDirty = true; }
	void setScale(const glm::vec3 &scale)		{ m_scale = scale;			m_matModelIsDirty = true; }

	void setColor(const glm::vec3 &color) { m_color = color; m_colorIsDirty = true; }

	void setAmbientColor(const glm::vec3 &ambientColor)		{ m_ambientColor = ambientColor; m_colorIsDirty = true; }
	void setDiffuseColor(const glm::vec3 &diffuseColor)		{ m_diffuseColor = diffuseColor; m_colorIsDirty = true; }
	void setSpecularColor(const glm::vec3 &specularColor)	{ m_specularColor = specularColor; m_colorIsDirty = true; }

	void setDiffuseTexture(int diffuseTexInd)	{ m_diffuseTexInd = diffuseTexInd; m_colorIsDirty = true; }
	void setSpecularTexture(int specularTexInd) { m_specularTexInd = specularTexInd; m_colorIsDirty = true; }

	void setShininess(float shininess)		{ m_shininess = shininess; m_colorIsDirty = true; }
	void setSaturation(float saturation)	{ m_saturation = saturation; m_colorIsDirty = true; }
	void setBrightness(float brightness)	{ m_brightness = brightness; m_colorIsDirty = true; }

	void setMaterialColor(
		const glm::vec3 &ambientColor,
		const glm::vec3 &diffuseColor,
		const glm::vec3 &specularColor,
		float shininess = 32.0f,
		float saturation = 1.0f,
		float brightness = 1.0f);

	void setMaterialTexture(
		int diffuseTexInd,
		int specularTexInd,
		float shininess = 32.0f,
		float saturation = 1.0f,
		float brightness = 1.0f);

	const glm::vec3& getPosition() { return m_position; }
	const glm::vec3& getRotation() { return m_rotation; }
	const glm::vec3& getScale() { return m_scale; }
	const glm::vec3& getColor() { return m_color; }

	const glm::vec3& getAmbientColor() { return m_ambientColor; }
	const glm::vec3& getDiffuseColor() { return m_diffuseColor; }
	const glm::vec3& getSpecularColor() { return m_specularColor; }

	int getDiffuseTexture() { return m_diffuseTexInd; }
	int getSpecularTexture() { return m_specularTexInd; }

	float getShininess() { return m_shininess; }
	float getSaturation() {return m_saturation; }
	float getBrightness() {return m_brightness; }
	
private:
	ModelPtr m_model;
	ShaderPtr m_shader;

	glm::vec3 m_position;
	glm::vec3 m_rotation;
	glm::vec3 m_scale;

	glm::vec3 m_color;

	glm::vec3 m_ambientColor;
	glm::vec3 m_diffuseColor;
	glm::vec3 m_specularColor;

	float m_shininess;
	float m_saturation;
	float m_brightness;

	int m_diffuseTexInd;
	int m_specularTexInd;

	bool m_matModelIsDirty;
	glm::mat4 m_matModel;
	bool m_colorIsDirty;
};

