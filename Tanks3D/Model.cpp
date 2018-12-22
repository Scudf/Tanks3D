#include "stdafx.h"
#include "Model.h"


Model::Model()
{
	glGenVertexArrays(1, &m_VAO);
	glGenBuffers(1, &m_VBO);
}

void Model::createVAO(
	const std::vector<GLfloat> &vector,
	int vectorSize,
	VAOconfig vaoConfig,
	int metric)
{
	glBindVertexArray(m_VAO);

	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * vectorSize, vector.data(), GL_STATIC_DRAW);

	GLvoid* offset;
	switch (vaoConfig)
	{
	case VAOconfig::TEXTURES:
		offset = (GLvoid*)(sizeof(GLfloat) * (5 + metric));
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, (vaoConfig + metric) * sizeof(GLfloat), offset);
		glEnableVertexAttribArray(2);

	case VAOconfig::NORMALS:
		offset = (GLvoid*)(sizeof(GLfloat) * (2 + metric));
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, (vaoConfig + metric) * sizeof(GLfloat), offset);
		glEnableVertexAttribArray(1);

	case VAOconfig::SIMPLE:
		glVertexAttribPointer(0, 2 + metric, GL_FLOAT, GL_FALSE, (vaoConfig + metric) * sizeof(GLfloat), (GLvoid*)0);
		glEnableVertexAttribArray(0);
	}
}

bool Model::createModel2V(const glm::vec2 &lineLenght)
{
	std::vector<GLfloat> lineVertices;

	lineVertices = {
				0.0f,			0.0f,
		lineLenght.x,	lineLenght.y,
	};

	createVAO(lineVertices, lineVertices.size(), SIMPLE, METRIC_2D);
	m_vaoConfig = SIMPLE;
	m_modelType = ModelType::V2;

	return true;
}

bool Model::createModel4V(
	VAOconfig vaoConfig,
	const glm::vec2 &planeSize,
	const TexCoords &texCoords)
{
	std::vector<GLfloat> planeVertices;
	
	switch (vaoConfig)
	{
	case TEXTURES:
		planeVertices = {
					0.0f,			0.0f,	0.0f, 0.0f, 1.0f,	texCoords.frontUV.x, texCoords.frontUV.z,
			 planeSize.x,			0.0f,	0.0f, 0.0f, 1.0f,	texCoords.frontUV.z, texCoords.frontUV.z,
					0.0f,	 planeSize.y,	0.0f, 0.0f, 1.0f,	texCoords.frontUV.x, texCoords.frontUV.y,
			 planeSize.x,	 planeSize.y,	0.0f, 0.0f, 1.0f,	texCoords.frontUV.z, texCoords.frontUV.y
		};
		break;

	case NORMALS:
		planeVertices = {
					0.0f,			0.0f,	0.0f, 0.0f, 1.0f,
			planeSize.x,			0.0f,	0.0f, 0.0f, 1.0f,
					0.0f,	 planeSize.y,	0.0f, 0.0f, 1.0f,
			planeSize.x,	 planeSize.y,	0.0f, 0.0f, 1.0f
		};
		break;

	case SIMPLE:
		planeVertices = {
					0.0f,			0.0f,
			planeSize.x,			0.0f,
					0.0f,	 planeSize.y,
			planeSize.x,	 planeSize.y
		};
		break;

	}

	createVAO(planeVertices, planeVertices.size(), vaoConfig, METRIC_2D);
	m_vaoConfig = vaoConfig;
	m_modelType = ModelType::V4;

	return true;
}

bool Model::createModel36V(
	VAOconfig vaoConfig,
	const glm::vec3 &cubeSize,
	const TexCoords &texCoords)
{
	std::vector<GLfloat> cubeVertices;

	float x = cubeSize.x / 2;
	float y = cubeSize.y / 2;
	float z = cubeSize.z / 2;

	switch (vaoConfig)
	{
	case TEXTURES:
		cubeVertices = {
			// Top side
			-x,  y, -z,  0.0f,  1.0f,  0.0f,  texCoords.topUV.x,   texCoords.topUV.w,
			 x,  y, -z,  0.0f,  1.0f,  0.0f,  texCoords.topUV.z,   texCoords.topUV.w,
			 x,  y,  z,  0.0f,  1.0f,  0.0f,  texCoords.topUV.z,   texCoords.topUV.y,
			 x,  y,  z,  0.0f,  1.0f,  0.0f,  texCoords.topUV.z,   texCoords.topUV.y,
			-x,  y,  z,  0.0f,  1.0f,  0.0f,  texCoords.topUV.x,   texCoords.topUV.y,
			-x,  y, -z,  0.0f,  1.0f,  0.0f,  texCoords.topUV.x,   texCoords.topUV.w,

			// Bottom side
			-x, -y, -z,  0.0f, -1.0f,  0.0f,  texCoords.bottomUV.x,  texCoords.bottomUV.w,
			 x, -y, -z,  0.0f, -1.0f,  0.0f,  texCoords.bottomUV.z,  texCoords.bottomUV.w,
			 x, -y,  z,  0.0f, -1.0f,  0.0f,  texCoords.bottomUV.z,  texCoords.bottomUV.y,
			 x, -y,  z,  0.0f, -1.0f,  0.0f,  texCoords.bottomUV.z,  texCoords.bottomUV.y,
			-x, -y,  z,  0.0f, -1.0f,  0.0f,  texCoords.bottomUV.x,  texCoords.bottomUV.y,
			-x, -y, -z,  0.0f, -1.0f,  0.0f,  texCoords.bottomUV.x,  texCoords.bottomUV.w,

			// Right side
			 x,  y,  z,  1.0f,  0.0f,  0.0f,  texCoords.rightUV.x, texCoords.rightUV.y,
			 x,  y, -z,  1.0f,  0.0f,  0.0f,  texCoords.rightUV.x, texCoords.rightUV.w,
			 x, -y, -z,  1.0f,  0.0f,  0.0f,  texCoords.rightUV.z, texCoords.rightUV.w,
			 x, -y, -z,  1.0f,  0.0f,  0.0f,  texCoords.rightUV.z, texCoords.rightUV.w,
			 x, -y,  z,  1.0f,  0.0f,  0.0f,  texCoords.rightUV.z, texCoords.rightUV.y,
			 x,  y,  z,  1.0f,  0.0f,  0.0f,  texCoords.rightUV.x, texCoords.rightUV.y,

			// Left side
			-x,  y,  z, -1.0f,  0.0f,  0.0f,  texCoords.leftUV.x,  texCoords.leftUV.y,
			-x,  y, -z, -1.0f,  0.0f,  0.0f,  texCoords.leftUV.x,  texCoords.leftUV.w,
			-x, -y, -z, -1.0f,  0.0f,  0.0f,  texCoords.leftUV.z,  texCoords.leftUV.w,
			-x, -y, -z, -1.0f,  0.0f,  0.0f,  texCoords.leftUV.z,  texCoords.leftUV.w,
			-x, -y,  z, -1.0f,  0.0f,  0.0f,  texCoords.leftUV.z,  texCoords.leftUV.y,
			-x,  y,  z, -1.0f,  0.0f,  0.0f,  texCoords.leftUV.x,  texCoords.leftUV.y,

			// Front side
			-x, -y,  z,  0.0f,  0.0f,  1.0f,  texCoords.frontUV.z, texCoords.frontUV.w,
			 x, -y,  z,  0.0f,  0.0f,  1.0f,  texCoords.frontUV.z, texCoords.frontUV.y,
			 x,  y,  z,  0.0f,  0.0f,  1.0f,  texCoords.frontUV.x, texCoords.frontUV.y,
			 x,  y,  z,  0.0f,  0.0f,  1.0f,  texCoords.frontUV.x, texCoords.frontUV.y,
			-x,  y,  z,  0.0f,  0.0f,  1.0f,  texCoords.frontUV.x, texCoords.frontUV.w,
			-x, -y,  z,  0.0f,  0.0f,  1.0f,  texCoords.frontUV.z, texCoords.frontUV.w,

			// Back side
			-x, -y, -z,  0.0f,  0.0f, -1.0f,  texCoords.backUV.z,  texCoords.backUV.w,
			 x, -y, -z,  0.0f,  0.0f, -1.0f,  texCoords.backUV.z,  texCoords.backUV.y,
			 x,  y, -z,  0.0f,  0.0f, -1.0f,  texCoords.backUV.x,  texCoords.backUV.y,
			 x,  y, -z,  0.0f,  0.0f, -1.0f,  texCoords.backUV.x,  texCoords.backUV.y,
			-x,  y, -z,  0.0f,  0.0f, -1.0f,  texCoords.backUV.x,  texCoords.backUV.w,
			-x, -y, -z,  0.0f,  0.0f, -1.0f,  texCoords.backUV.z,  texCoords.backUV.w
		};

		break;

	case NORMALS:
		cubeVertices = {
			// Top side
			-x,  y, -z,  0.0f,  1.0f,  0.0f,
			 x,  y, -z,  0.0f,  1.0f,  0.0f,
			 x,  y,  z,  0.0f,  1.0f,  0.0f,
			 x,  y,  z,  0.0f,  1.0f,  0.0f,
			-x,  y,  z,  0.0f,  1.0f,  0.0f,
			-x,  y, -z,  0.0f,  1.0f,  0.0f,

			// Bottom side
			-x, -y, -z,  0.0f, -1.0f,  0.0f,
			 x, -y, -z,  0.0f, -1.0f,  0.0f,
			 x, -y,  z,  0.0f, -1.0f,  0.0f,
			 x, -y,  z,  0.0f, -1.0f,  0.0f,
			-x, -y,  z,  0.0f, -1.0f,  0.0f,
			-x, -y, -z,  0.0f, -1.0f,  0.0f,
			
			// Right side
			 x,  y,  z,  1.0f,  0.0f,  0.0f,
			 x,  y, -z,  1.0f,  0.0f,  0.0f,
			 x, -y, -z,  1.0f,  0.0f,  0.0f,
			 x, -y, -z,  1.0f,  0.0f,  0.0f,
			 x, -y,  z,  1.0f,  0.0f,  0.0f,
			 x,  y,  z,  1.0f,  0.0f,  0.0f,
			
			// Left side
			-x,  y,  z, -1.0f,  0.0f,  0.0f,
			-x,  y, -z, -1.0f,  0.0f,  0.0f,
			-x, -y, -z, -1.0f,  0.0f,  0.0f,
			-x, -y, -z, -1.0f,  0.0f,  0.0f,
			-x, -y,  z, -1.0f,  0.0f,  0.0f,
			-x,  y,  z, -1.0f,  0.0f,  0.0f,
			
			// Front side
			-x, -y,  z,  0.0f,  0.0f,  1.0f,
			 x, -y,  z,  0.0f,  0.0f,  1.0f,
			 x,  y,  z,  0.0f,  0.0f,  1.0f,
			 x,  y,  z,  0.0f,  0.0f,  1.0f,
			-x,  y,  z,  0.0f,  0.0f,  1.0f,
			-x, -y,  z,  0.0f,  0.0f,  1.0f,
			
			// Back side
			-x, -y, -z,  0.0f,  0.0f, -1.0f,
			 x, -y, -z,  0.0f,  0.0f, -1.0f,
			 x,  y, -z,  0.0f,  0.0f, -1.0f,
			 x,  y, -z,  0.0f,  0.0f, -1.0f,
			-x,  y, -z,  0.0f,  0.0f, -1.0f,
			-x, -y, -z,  0.0f,  0.0f, -1.0f
						  
		};

		break;

	case SIMPLE:
		cubeVertices = {
			// Top side
			-x,  y, -z,
			 x,  y, -z,
			 x,  y,  z,
			 x,  y,  z,
			-x,  y,  z,
			-x,  y, -z,

			// Bottom side
			-x, -y, -z,
			 x, -y, -z,
			 x, -y,  z,
			 x, -y,  z,
			-x, -y,  z,
			-x, -y, -z,

			// Right side
			 x,  y,  z,
			 x,  y, -z,
			 x, -y, -z,
			 x, -y, -z,
			 x, -y,  z,
			 x,  y,  z,

			// Left side
			-x,  y,  z,
			-x,  y, -z,
			-x, -y, -z,
			-x, -y, -z,
			-x, -y,  z,
			-x,  y,  z,

			// Front side
			-x, -y,  z,
			 x, -y,  z,
			 x,  y,  z,
			 x,  y,  z,
			-x,  y,  z,
			-x, -y,  z,

			// Back side
			-x, -y, -z,
			 x, -y, -z,
			 x,  y, -z,
			 x,  y, -z,
			-x,  y, -z,
			-x, -y, -z,
		};

		break;
	}

	createVAO(cubeVertices, cubeVertices.size(), vaoConfig, METRIC_3D);
	m_vaoConfig = vaoConfig;
	m_modelType = ModelType::V36;

	return true;
}

bool Model::createModel72V(
	VAOconfig vaoConfig,
	const glm::vec3 &firstCubeSize,
	const glm::vec3 &secondCubeSize,
	const TexCoords &firstTexCoords,
	const TexCoords &secondTexCoords)
{
	std::vector<GLfloat> cubeVertices;

	float x = firstCubeSize.x / 2;
	float y = firstCubeSize.y / 2;
	float z = firstCubeSize.z / 2;

	float a = secondCubeSize.x + x;
	float b = secondCubeSize.y / 2;
	float c = secondCubeSize.z / 2;

	switch (vaoConfig)
	{
	case TEXTURES:
		cubeVertices = {
			// Top side
			-x,  y, -z,  0.0f,  1.0f,  0.0f,  firstTexCoords.topUV.z,   firstTexCoords.topUV.w,
			 x,  y, -z,  0.0f,  1.0f,  0.0f,  firstTexCoords.topUV.x,   firstTexCoords.topUV.w,
			 x,  y,  z,  0.0f,  1.0f,  0.0f,  firstTexCoords.topUV.x,   firstTexCoords.topUV.y,
			 x,  y,  z,  0.0f,  1.0f,  0.0f,  firstTexCoords.topUV.x,   firstTexCoords.topUV.y,
			-x,  y,  z,  0.0f,  1.0f,  0.0f,  firstTexCoords.topUV.z,   firstTexCoords.topUV.y,
			-x,  y, -z,  0.0f,  1.0f,  0.0f,  firstTexCoords.topUV.z,   firstTexCoords.topUV.w,
											 
			// Bottom side					 
			-x, -y, -z,  0.0f, -1.0f,  0.0f,  firstTexCoords.bottomUV.z,  firstTexCoords.bottomUV.w,
			 x, -y, -z,  0.0f, -1.0f,  0.0f,  firstTexCoords.bottomUV.x,  firstTexCoords.bottomUV.w,
			 x, -y,  z,  0.0f, -1.0f,  0.0f,  firstTexCoords.bottomUV.x,  firstTexCoords.bottomUV.y,
			 x, -y,  z,  0.0f, -1.0f,  0.0f,  firstTexCoords.bottomUV.x,  firstTexCoords.bottomUV.y,
			-x, -y,  z,  0.0f, -1.0f,  0.0f,  firstTexCoords.bottomUV.z,  firstTexCoords.bottomUV.y,
			-x, -y, -z,  0.0f, -1.0f,  0.0f,  firstTexCoords.bottomUV.z,  firstTexCoords.bottomUV.w,
											 
			// Right side					 
			 x,  y,  z,  1.0f,  0.0f,  0.0f,  firstTexCoords.rightUV.x, firstTexCoords.rightUV.y,
			 x,  y, -z,  1.0f,  0.0f,  0.0f,  firstTexCoords.rightUV.x, firstTexCoords.rightUV.w,
			 x, -y, -z,  1.0f,  0.0f,  0.0f,  firstTexCoords.rightUV.z, firstTexCoords.rightUV.w,
			 x, -y, -z,  1.0f,  0.0f,  0.0f,  firstTexCoords.rightUV.z, firstTexCoords.rightUV.w,
			 x, -y,  z,  1.0f,  0.0f,  0.0f,  firstTexCoords.rightUV.z, firstTexCoords.rightUV.y,
			 x,  y,  z,  1.0f,  0.0f,  0.0f,  firstTexCoords.rightUV.x, firstTexCoords.rightUV.y,
											 
			// Left side					
			-x,  y,  z, -1.0f,  0.0f,  0.0f,  firstTexCoords.leftUV.x,  firstTexCoords.leftUV.y,
			-x,  y, -z, -1.0f,  0.0f,  0.0f,  firstTexCoords.leftUV.x,  firstTexCoords.leftUV.w,
			-x, -y, -z, -1.0f,  0.0f,  0.0f,  firstTexCoords.leftUV.z,  firstTexCoords.leftUV.w,
			-x, -y, -z, -1.0f,  0.0f,  0.0f,  firstTexCoords.leftUV.z,  firstTexCoords.leftUV.w,
			-x, -y,  z, -1.0f,  0.0f,  0.0f,  firstTexCoords.leftUV.z,  firstTexCoords.leftUV.y,
			-x,  y,  z, -1.0f,  0.0f,  0.0f,  firstTexCoords.leftUV.x,  firstTexCoords.leftUV.y,
											  
			// Front side					
			-x, -y,  z,  0.0f,  0.0f,  1.0f,  firstTexCoords.frontUV.z, firstTexCoords.frontUV.y,
			 x, -y,  z,  0.0f,  0.0f,  1.0f,  firstTexCoords.frontUV.x, firstTexCoords.frontUV.y,
			 x,  y,  z,  0.0f,  0.0f,  1.0f,  firstTexCoords.frontUV.x, firstTexCoords.frontUV.w,
			 x,  y,  z,  0.0f,  0.0f,  1.0f,  firstTexCoords.frontUV.x, firstTexCoords.frontUV.w,
			-x,  y,  z,  0.0f,  0.0f,  1.0f,  firstTexCoords.frontUV.z, firstTexCoords.frontUV.w,
			-x, -y,  z,  0.0f,  0.0f,  1.0f,  firstTexCoords.frontUV.z, firstTexCoords.frontUV.y,
											
			// Back side					 
			-x, -y, -z,  0.0f,  0.0f, -1.0f,  firstTexCoords.frontUV.z, firstTexCoords.frontUV.y,
			 x, -y, -z,  0.0f,  0.0f, -1.0f,  firstTexCoords.frontUV.x, firstTexCoords.frontUV.y,
			 x,  y, -z,  0.0f,  0.0f, -1.0f,  firstTexCoords.frontUV.x, firstTexCoords.frontUV.w,
			 x,  y, -z,  0.0f,  0.0f, -1.0f,  firstTexCoords.frontUV.x, firstTexCoords.frontUV.w,
			-x,  y, -z,  0.0f,  0.0f, -1.0f,  firstTexCoords.frontUV.z, firstTexCoords.frontUV.w,
			-x, -y, -z,  0.0f,  0.0f, -1.0f,  firstTexCoords.frontUV.z, firstTexCoords.frontUV.y,

			// Second top side
			 x,  b, -c,  0.0f,  1.0f,  0.0f,  secondTexCoords.topUV.x,   secondTexCoords.topUV.w,
			 a,  b, -c,  0.0f,  1.0f,  0.0f,  secondTexCoords.topUV.x,   secondTexCoords.topUV.y,
			 a,  b,  c,  0.0f,  1.0f,  0.0f,  secondTexCoords.topUV.z,   secondTexCoords.topUV.y,
			 a,  b,  c,  0.0f,  1.0f,  0.0f,  secondTexCoords.topUV.z,   secondTexCoords.topUV.y,
			 x,  b,  c,  0.0f,  1.0f,  0.0f,  secondTexCoords.topUV.z,   secondTexCoords.topUV.w,
			 x,  b, -c,  0.0f,  1.0f,  0.0f,  secondTexCoords.topUV.x,   secondTexCoords.topUV.w,

			// Second bottom side
			 x, -b, -c,  0.0f, -1.0f,  0.0f,  secondTexCoords.bottomUV.x,  secondTexCoords.bottomUV.w,
			 a, -b, -c,  0.0f, -1.0f,  0.0f,  secondTexCoords.bottomUV.x,  secondTexCoords.bottomUV.y,
			 a, -b,  c,  0.0f, -1.0f,  0.0f,  secondTexCoords.bottomUV.z,  secondTexCoords.bottomUV.y,
			 a, -b,  c,  0.0f, -1.0f,  0.0f,  secondTexCoords.bottomUV.z,  secondTexCoords.bottomUV.y,
			 x, -b,  c,  0.0f, -1.0f,  0.0f,  secondTexCoords.bottomUV.z,  secondTexCoords.bottomUV.w,
			 x, -b, -c,  0.0f, -1.0f,  0.0f,  secondTexCoords.bottomUV.x,  secondTexCoords.bottomUV.w,

			// Second right side
			 a,  b,  c,  1.0f,  0.0f,  0.0f,  secondTexCoords.rightUV.x, secondTexCoords.rightUV.y,
			 a,  b, -c,  1.0f,  0.0f,  0.0f,  secondTexCoords.rightUV.x, secondTexCoords.rightUV.w,
			 a, -b, -c,  1.0f,  0.0f,  0.0f,  secondTexCoords.rightUV.z, secondTexCoords.rightUV.w,
			 a, -b, -c,  1.0f,  0.0f,  0.0f,  secondTexCoords.rightUV.z, secondTexCoords.rightUV.w,
			 a, -b,  c,  1.0f,  0.0f,  0.0f,  secondTexCoords.rightUV.z, secondTexCoords.rightUV.y,
			 a,  b,  c,  1.0f,  0.0f,  0.0f,  secondTexCoords.rightUV.x, secondTexCoords.rightUV.y,

			// Second left side
			 x,  b,  c, -1.0f,  0.0f,  0.0f,  secondTexCoords.leftUV.x,  secondTexCoords.leftUV.y,
			 x,  b, -c, -1.0f,  0.0f,  0.0f,  secondTexCoords.leftUV.x,  secondTexCoords.leftUV.w,
			 x, -b, -c, -1.0f,  0.0f,  0.0f,  secondTexCoords.leftUV.z,  secondTexCoords.leftUV.w,
			 x, -b, -c, -1.0f,  0.0f,  0.0f,  secondTexCoords.leftUV.z,  secondTexCoords.leftUV.w,
			 x, -b,  c, -1.0f,  0.0f,  0.0f,  secondTexCoords.leftUV.z,  secondTexCoords.leftUV.y,
			 x,  b,  c, -1.0f,  0.0f,  0.0f,  secondTexCoords.leftUV.x,  secondTexCoords.leftUV.y,

			// Second front side
			 x, -b,  c,  0.0f,  0.0f,  1.0f,  secondTexCoords.frontUV.z, secondTexCoords.frontUV.w,
			 a, -b,  c,  0.0f,  0.0f,  1.0f,  secondTexCoords.frontUV.z, secondTexCoords.frontUV.y,
			 a,  b,  c,  0.0f,  0.0f,  1.0f,  secondTexCoords.frontUV.x, secondTexCoords.frontUV.y,
			 a,  b,  c,  0.0f,  0.0f,  1.0f,  secondTexCoords.frontUV.x, secondTexCoords.frontUV.y,
			 x,  b,  c,  0.0f,  0.0f,  1.0f,  secondTexCoords.frontUV.x, secondTexCoords.frontUV.w,
			 x, -b,  c,  0.0f,  0.0f,  1.0f,  secondTexCoords.frontUV.z, secondTexCoords.frontUV.w,

			// Second back side
			 x, -b, -c,  0.0f,  0.0f, -1.0f,  secondTexCoords.backUV.z,  secondTexCoords.backUV.w,
			 a, -b, -c,  0.0f,  0.0f, -1.0f,  secondTexCoords.backUV.z,  secondTexCoords.backUV.y,
			 a,  b, -c,  0.0f,  0.0f, -1.0f,  secondTexCoords.backUV.x,  secondTexCoords.backUV.y,
			 a,  b, -c,  0.0f,  0.0f, -1.0f,  secondTexCoords.backUV.x,  secondTexCoords.backUV.y,
			 x,  b, -c,  0.0f,  0.0f, -1.0f,  secondTexCoords.backUV.x,  secondTexCoords.backUV.w,
			 x, -b, -c,  0.0f,  0.0f, -1.0f,  secondTexCoords.backUV.z,  secondTexCoords.backUV.w
		};

		break;

	case NORMALS:
		cubeVertices = {
			// Top side
			-x,  y, -z,  0.0f,  1.0f,  0.0f,
			 x,  y, -z,  0.0f,  1.0f,  0.0f,
			 x,  y,  z,  0.0f,  1.0f,  0.0f,
			 x,  y,  z,  0.0f,  1.0f,  0.0f,
			-x,  y,  z,  0.0f,  1.0f,  0.0f,
			-x,  y, -z,  0.0f,  1.0f,  0.0f,
											
			// Bottom side					
			-x, -y, -z,  0.0f, -1.0f,  0.0f,
			 x, -y, -z,  0.0f, -1.0f,  0.0f,
			 x, -y,  z,  0.0f, -1.0f,  0.0f,
			 x, -y,  z,  0.0f, -1.0f,  0.0f,
			-x, -y,  z,  0.0f, -1.0f,  0.0f,
			-x, -y, -z,  0.0f, -1.0f,  0.0f,
											
			// Right side					
			 x,  y,  z,  1.0f,  0.0f,  0.0f,
			 x,  y, -z,  1.0f,  0.0f,  0.0f,
			 x, -y, -z,  1.0f,  0.0f,  0.0f,
			 x, -y, -z,  1.0f,  0.0f,  0.0f,
			 x, -y,  z,  1.0f,  0.0f,  0.0f,
			 x,  y,  z,  1.0f,  0.0f,  0.0f,
											
			// Left side					
			-x,  y,  z, -1.0f,  0.0f,  0.0f,
			-x,  y, -z, -1.0f,  0.0f,  0.0f,
			-x, -y, -z, -1.0f,  0.0f,  0.0f,
			-x, -y, -z, -1.0f,  0.0f,  0.0f,
			-x, -y,  z, -1.0f,  0.0f,  0.0f,
			-x,  y,  z, -1.0f,  0.0f,  0.0f,
											
			// Front side					
			-x, -y,  z,  0.0f,  0.0f,  1.0f,
			 x, -y,  z,  0.0f,  0.0f,  1.0f,
			 x,  y,  z,  0.0f,  0.0f,  1.0f,
			 x,  y,  z,  0.0f,  0.0f,  1.0f,
			-x,  y,  z,  0.0f,  0.0f,  1.0f,
			-x, -y,  z,  0.0f,  0.0f,  1.0f,
											
			// Back side					
			-x, -y, -z,  0.0f,  0.0f, -1.0f,
			 x, -y, -z,  0.0f,  0.0f, -1.0f,
			 x,  y, -z,  0.0f,  0.0f, -1.0f,
			 x,  y, -z,  0.0f,  0.0f, -1.0f,
			-x,  y, -z,  0.0f,  0.0f, -1.0f,
			-x, -y, -z,  0.0f,  0.0f, -1.0f,

			// Second top side
			 x,  b, -c,  0.0f,  1.0f,  0.0f,
			 a,  b, -c,  0.0f,  1.0f,  0.0f,
			 a,  b,  c,  0.0f,  1.0f,  0.0f,
			 a,  b,  c,  0.0f,  1.0f,  0.0f,
			 x,  b,  c,  0.0f,  1.0f,  0.0f,
			 x,  b, -c,  0.0f,  1.0f,  0.0f,

			// Second bottom side
			 x, -b, -c,  0.0f, -1.0f,  0.0f,
			 a, -b, -c,  0.0f, -1.0f,  0.0f,
			 a, -b,  c,  0.0f, -1.0f,  0.0f,
			 a, -b,  c,  0.0f, -1.0f,  0.0f,
			 x, -b,  c,  0.0f, -1.0f,  0.0f,
			 x, -b, -c,  0.0f, -1.0f,  0.0f,

			// Second right side
			 a,  b,  c,  1.0f,  0.0f,  0.0f,
			 a,  b, -c,  1.0f,  0.0f,  0.0f,
			 a, -b, -c,  1.0f,  0.0f,  0.0f,
			 a, -b, -c,  1.0f,  0.0f,  0.0f,
			 a, -b,  c,  1.0f,  0.0f,  0.0f,
			 a,  b,  c,  1.0f,  0.0f,  0.0f,

			// Second left side
			 x,  b,  c, -1.0f,  0.0f,  0.0f,
			 x,  b, -c, -1.0f,  0.0f,  0.0f,
			 x, -b, -c, -1.0f,  0.0f,  0.0f,
			 x, -b, -c, -1.0f,  0.0f,  0.0f,
			 x, -b,  c, -1.0f,  0.0f,  0.0f,
			 x,  b,  c, -1.0f,  0.0f,  0.0f,

			// Second front side
			 x, -b,  c,  0.0f,  0.0f,  1.0f,
			 a, -b,  c,  0.0f,  0.0f,  1.0f,
			 a,  b,  c,  0.0f,  0.0f,  1.0f,
			 a,  b,  c,  0.0f,  0.0f,  1.0f,
			 x,  b,  c,  0.0f,  0.0f,  1.0f,
			 x, -b,  c,  0.0f,  0.0f,  1.0f,

			// Second back side
			 x, -b, -c,  0.0f,  0.0f, -1.0f,
			 a, -b, -c,  0.0f,  0.0f, -1.0f,
			 a,  b, -c,  0.0f,  0.0f, -1.0f,
			 a,  b, -c,  0.0f,  0.0f, -1.0f,
			 x,  b, -c,  0.0f,  0.0f, -1.0f,
			 x, -b, -c,  0.0f,  0.0f, -1.0f
		};

		break;

	case SIMPLE:
		cubeVertices = {
			// Top side
			-x,  y, -z,
			 x,  y, -z,
			 x,  y,  z,
			 x,  y,  z,
			-x,  y,  z,
			-x,  y, -z,

			// Bottom side
			-x, -y, -z,
			 x, -y, -z,
			 x, -y,  z,
			 x, -y,  z,
			-x, -y,  z,
			-x, -y, -z,

			// Right side
			 x,  y,  z,
			 x,  y, -z,
			 x, -y, -z,
			 x, -y, -z,
			 x, -y,  z,
			 x,  y,  z,

			// Left side
			-x,  y,  z,
			-x,  y, -z,
			-x, -y, -z,
			-x, -y, -z,
			-x, -y,  z,
			-x,  y,  z,

			// Front side
			-x, -y,  z,
			 x, -y,  z,
			 x,  y,  z,
			 x,  y,  z,
			-x,  y,  z,
			-x, -y,  z,

			// Back side
			-x, -y, -z,
			 x, -y, -z,
			 x,  y, -z,
			 x,  y, -z,
			-x,  y, -z,
			-x, -y, -z,

			// Second top side
			 x,  b, -c,
			 a,  b, -c,
			 a,  b,  c,
			 a,  b,  c,
			 x,  b,  c,
			 x,  b, -c,

			// Second bottom side
			 x, -b, -c,
			 a, -b, -c,
			 a, -b,  c,
			 a, -b,  c,
			 x, -b,  c,
			 x, -b, -c,

			// Second right side
			 a,  b,  c,
			 a,  b, -c,
			 a, -b, -c,
			 a, -b, -c,
			 a, -b,  c,
			 a,  b,  c,

			// Second left side
			 x,  b,  c,
			 x,  b, -c,
			 x, -b, -c,
			 x, -b, -c,
			 x, -b,  c,
			 x,  b,  c,

			// Second front sid
			 x, -b,  c,
			 a, -b,  c,
			 a,  b,  c,
			 a,  b,  c,
			 x,  b,  c,
			 x, -b,  c,

			// Second back side
			 x, -b, -c,
			 a, -b, -c,
			 a,  b, -c,
			 a,  b, -c,
			 x,  b, -c,
			 x, -b, -c,
		};

		break;
	}

		createVAO(cubeVertices, cubeVertices.size(), vaoConfig, METRIC_3D);
		m_vaoConfig = vaoConfig;
		m_modelType = ModelType::V72;

		return true;
}