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


const float BLOCK_SIZE = 100.0f;

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

	void setLightDirection(const glm::vec3 &lightDir) { m_lightDirection = lightDir; m_lightIsDirty = true; }
	void setLightAmbient(const glm::vec3 &lightAmb) { m_lightAmbient = lightAmb; m_lightIsDirty = true; }
	void setLightDiffuse(const glm::vec3 &lightDiff) { m_lightDiffuse = lightDiff; m_lightIsDirty = true; }
	void setLightSpecular(const glm::vec3 &lightSpec) { m_lightSpecular = lightSpec; m_lightIsDirty = true; }
	void setLightIsDirty(bool isDirty) { m_lightIsDirty = isDirty; }

	const int getWidth() const { return m_width; }
	const int getHeight() const { return m_height; }

	GLFWwindow* getWindow() const { return m_window; }
	const CameraPtr& getCamera() const { return m_camera; }

	GLuint getGLTexture(const std::string &imagePath) { return m_textures[imagePath]; }
	const ShaderPtr& getGLShader(const std::string &shaderName) { return m_shaders[shaderName]; }
	const ModelPtr&	 getModel(const std::string &modelName) { return m_models[modelName]; }

	const glm::mat4& getMatView() const { return m_matView; }
	const glm::mat4& getMatPersp() const { return m_matPersp; }

	const glm::vec3& getLightDirection() const { return m_lightDirection; }
	const glm::vec3& getLightAmbient() const { return m_lightAmbient; }
	const glm::vec3& getLightDiffuse() const { return m_lightDiffuse; }
	const glm::vec3& getLightSpecular() const { return m_lightSpecular; }
	bool getLightIsDirty() const { return m_lightIsDirty; }

	void addObject(const GraphicObjPtr& object);
	void removeObject(const GraphicObjPtr& object);
	void removeObject(GraphicObj* object);

private:
	int m_width;
	int m_height;

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

