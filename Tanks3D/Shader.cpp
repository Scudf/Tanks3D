#include "stdafx.h"
#include "Shader.h"


Shader::Shader(
	const char* vertexPath,
	const char* fragmentPath,
	const char* geometryPath)
{
	m_vertexPath = vertexPath;
	m_fragmentPath = fragmentPath;
	if (geometryPath != nullptr)
		m_geometryPath = geometryPath;

	std::string vertexShaderFile;
	std::string fragmentShaderFile;
	std::string geometryShaderFile;

	try
	{
		std::ifstream ifsV(vertexPath);
		vertexShaderFile.assign((std::istreambuf_iterator<char>(ifsV)),
			(std::istreambuf_iterator<char>()));

		std::ifstream ifsF(fragmentPath);
		fragmentShaderFile.assign((std::istreambuf_iterator<char>(ifsF)),
			(std::istreambuf_iterator<char>()));

		if (geometryPath != nullptr)
		{
			std::ifstream ifsG(geometryPath);
			geometryShaderFile.assign((std::istreambuf_iterator<char>(ifsG)),
				(std::istreambuf_iterator<char>()));
		}
	}
	catch (std::ifstream::failure e)
	{
		std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
	}

	const char* vertexShaderStr = vertexShaderFile.c_str();
	const char* fragmentShaderStr = fragmentShaderFile.c_str();
	const char* geometryShaderStr = geometryShaderFile.c_str();

	unsigned int vertexShader, fragmentShader, geometryShader;

	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderStr, NULL);
	glCompileShader(vertexShader);
	checkCompileErrors(vertexShader, "VERTEX");

	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderStr, NULL);
	glCompileShader(fragmentShader);
	checkCompileErrors(fragmentShader, "FRAGMENT");

	if (geometryPath != nullptr)
	{
		geometryShader = glCreateShader(GL_GEOMETRY_SHADER);
		glShaderSource(geometryShader, 1, &geometryShaderStr, NULL);
		glCompileShader(geometryShader);
		checkCompileErrors(geometryShader, "GEOMETRY");
	}

	m_shaderProgram = glCreateProgram();

	glAttachShader(m_shaderProgram, vertexShader);
	glAttachShader(m_shaderProgram, fragmentShader);
	if (geometryPath != nullptr)
		glAttachShader(m_shaderProgram, geometryShader);
	glLinkProgram(m_shaderProgram);
	checkCompileErrors(m_shaderProgram, "PROGRAM");

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
	if (geometryPath != nullptr)
		glDeleteShader(geometryShader);
}

bool Shader::hasUniform(const char* shaderName)
{
	if (glGetUniformLocation(m_shaderProgram, shaderName) == -1)
		return false;

	return true;
}

void Shader::checkCompileErrors(GLuint shader, std::string type)
{
	GLint success;
	GLchar infoLog[1024];
	if (type != "PROGRAM")
	{
		glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(shader, 1024, NULL, infoLog);
			std::cout << m_vertexPath << " - " << m_fragmentPath << '\n';
			std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
		}
	}
	else
	{
		glGetProgramiv(shader, GL_LINK_STATUS, &success);
		if (!success)
		{
			glGetProgramInfoLog(shader, 1024, NULL, infoLog);
			std::cout << m_vertexPath << " - " << m_fragmentPath << '\n';
			std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
		}
	}
}