#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <string>
#include <vector>
#include <map>
#include "Camera.h"
#include "Shader.h"
#include "Model.h"
#include "GraphicObj.h"


class GraphicScene
{
public:
	GraphicScene();
	~GraphicScene();

	bool initialize();
	bool update(float dt);

	void loadTexture(const std::string &imagePath);
	void loadShader(
		const std::string &shaderName,
		const std::string &vertexFileName,
		const std::string &fragmentFileName);

	void setLightDirection(const glm::vec3 &lightDir)	{ m_lightDirection = lightDir; m_lightIsDirty = true; }
	void setLightAmbient(const glm::vec3 &lightAmb)		{ m_lightAmbient = lightAmb; m_lightIsDirty = true; }
	void setLightDiffuse(const glm::vec3 &lightDiff)	{ m_lightDiffuse = lightDiff; m_lightIsDirty = true; }
	void setLightSpecular(const glm::vec3 &lightSpec)	{ m_lightSpecular = lightSpec; m_lightIsDirty = true; }
	void setLightIsDirty(bool isDirty)					{ m_lightIsDirty = isDirty; }

	bool getLightIsDirty() { return m_lightIsDirty; }
	
	GLFWwindow*			getWindow()	{ return m_window; }
	const CameraPtr&	getCamera()	{ return m_camera; }
	
	GLuint			 getGLTexture(const std::string &imagePath)	{ return m_textures[imagePath]; }
	const ShaderPtr& getGLShader(const std::string &shaderName)	{ return m_shaders[shaderName]; }
	const ModelPtr&	 getModel(const std::string &modelName)		{ return m_models[modelName]; }
	
	glm::mat4 getMatView()		{ return m_matView; }
	glm::mat4 getMatPersp()		{ return m_matPersp; }
	
	glm::vec3 getLightDirection()	{ return m_lightDirection; }
	glm::vec3 getLightAmbient()		{ return m_lightAmbient; }
	glm::vec3 getLightDiffuse()		{ return m_lightDiffuse; }
	glm::vec3 getLightSpecular()	{ return m_lightSpecular; }
	
	void addObject(const GraphicObjPtr& object);
	void removeObject(const GraphicObjPtr& object);
	void removeObject(GraphicObj* object);
	
private:
	GLFWwindow* m_window;
	CameraPtr m_camera;

	std::map<std::string, GLuint> m_textures;
	std::map<std::string, ShaderPtr> m_shaders;
	std::map<std::string, ModelPtr> m_models;

	glm::mat4 m_matView;
	glm::mat4 m_matPersp;

	glm::vec3 m_lightDirection;
	glm::vec3 m_lightAmbient;
	glm::vec3 m_lightDiffuse;
	glm::vec3 m_lightSpecular;
	bool m_lightIsDirty;

	std::vector<GraphicObjPtr> m_objects;

	void loadModels();
};

