#pragma once

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <memory>


using ShaderPtr = std::shared_ptr<class Shader>;

class Shader
{
public:
	Shader(
		const char* vertexPath,
		const char* fragmentPath,
		const char* geometryPath = nullptr);

	static ShaderPtr Create(
		const char* vertexPath,
		const char* fragmentPath,
		const char* geometryPath = nullptr) { return std::make_shared<Shader>(vertexPath, fragmentPath, geometryPath); }

	void Use() { glUseProgram(m_shaderProgram); }

	GLuint getShaderProgram() { return m_shaderProgram; }

	void setUniform(const char* name, bool value)	{ glUniform1i(glGetUniformLocation(m_shaderProgram, name), (int)value); }
	void setUniform(const char* name, int value)	{ glUniform1i(glGetUniformLocation(m_shaderProgram, name), value); }
	void setUniform(const char* name, float value)	{ glUniform1f(glGetUniformLocation(m_shaderProgram, name), value); }
						 
	void setUniform(const char* name, float x, float y)						{ glUniform2f(glGetUniformLocation(m_shaderProgram, name), x, y); }
	void setUniform(const char* name, float x, float y, float z)			{ glUniform3f(glGetUniformLocation(m_shaderProgram, name), x, y, z); }
	void setUniform(const char* name, float x, float y, float z, float w)	{ glUniform4f(glGetUniformLocation(m_shaderProgram, name), x, y, z, w); }
						
	void setUniform(const char* name, const glm::vec2 &value) { glUniform2fv(glGetUniformLocation(m_shaderProgram, name), 1, &value[0]); }
	void setUniform(const char* name, const glm::vec3 &value) { glUniform3fv(glGetUniformLocation(m_shaderProgram, name), 1, &value[0]); }
	void setUniform(const char* name, const glm::vec4 &value) { glUniform4fv(glGetUniformLocation(m_shaderProgram, name), 1, &value[0]); }
					
	void setUniform(const char* name, const glm::mat2 &value) { glUniformMatrix2fv(glGetUniformLocation(m_shaderProgram, name), 1, GL_FALSE, &value[0][0]); }
	void setUniform(const char* name, const glm::mat3 &value) { glUniformMatrix3fv(glGetUniformLocation(m_shaderProgram, name), 1, GL_FALSE, &value[0][0]); }
	void setUniform(const char* name, const glm::mat4 &value) { glUniformMatrix4fv(glGetUniformLocation(m_shaderProgram, name), 1, GL_FALSE, &value[0][0]); }
						
	bool hasUniform(const char* shaderName);

private:
	GLuint m_shaderProgram;

	std::string m_vertexPath;
	std::string m_fragmentPath;
	std::string m_geometryPath;

	void checkCompileErrors(GLuint shader, std::string type);
};

