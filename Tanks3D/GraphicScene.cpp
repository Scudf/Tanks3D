#include "stdafx.h"
#include "GraphicScene.h"
#include <iostream>
#include <glm/gtc/matrix_transform.hpp>
#include "pngLoader.h"
#include "constants.h"


const int WIDTH = 1024;
const int HEIGHT = 768;

GraphicScene::GraphicScene()
	: m_width(WIDTH)
	, m_height(HEIGHT)
	, m_lightDirection(0.0f, -0.5f, -1.0f)
	, m_lightAmbient(0.2f, 0.2f, 0.2f)
	, m_lightDiffuse(0.5f, 0.5f, 0.5f)
	, m_lightSpecular(1.0f, 1.0f, 1.0f)
	, m_lightIsDirty(true)
{

}

GraphicScene::~GraphicScene()
{
	glfwTerminate();
}

bool GraphicScene::initialize()
{
	glfwInit();

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
	glfwWindowHint(GLFW_DEPTH_BITS, 24);

	m_window = glfwCreateWindow(m_width, m_height, "Tanks3D", nullptr, nullptr);
	if (m_window == nullptr)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return false;
	}
	glfwMakeContextCurrent(m_window);

	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK)
	{
		std::cout << "Failed to initialize GLEW" << std::endl;
		return false;
	}

	GLint width, height;
	glfwGetFramebufferSize(m_window, &width, &height);
	glViewport(0, 0, width, height);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_DEPTH_TEST);

	m_camera = Camera::Create(glm::vec3(1600.0f, 1600.0f, 3200.0f));

	loadTexture("textures/Ground_Diffuse.png");
	loadTexture("textures/Ground_Specular.png");
	loadTexture("textures/Caterpillar_Diffuse.png");
	loadTexture("textures/Caterpillar_Specular.png");
	loadTexture("textures/Tower_Diffuse.png");
	loadTexture("textures/Box_Diffuse.png");
	loadTexture("textures/Box_Specular.png");
	loadTexture("textures/Tree_Diffuse.png");
	loadTexture("textures/Tree_Specular.png");
	loadTexture("textures/Fence_Diffuse.png");
	loadTexture("textures/Fence_Specular.png");

	loadShader("simpleShader", "shaders/simpleShaderV.glsl", "shaders/simpleShaderF.glsl");
	loadShader("normalsShader", "shaders/normalsShaderV.glsl", "shaders/normalsShaderF.glsl");
	loadShader("texturesShader", "shaders/texturesShaderV.glsl", "shaders/texturesShaderF.glsl");

	loadModels();
	return true;
}

bool GraphicScene::update(float dt)
{
	if (glfwWindowShouldClose(m_window))
		return false;

	glfwPollEvents();

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	if(m_camera->getIsFree())
		m_matView = m_camera->getFreeViewMatrix();
	else
		m_matView = m_camera->getTargetViewMatrix();

	m_matPersp = glm::perspective(glm::radians(m_camera->getZoom()), (GLfloat)m_width / m_height, 0.1f, 12800.0f);

	std::vector<GraphicObjPtr> temp = m_objects;
	for (GraphicObjPtr object : temp)
		object->render();

	m_lightIsDirty = false;

	glfwSwapBuffers(m_window);

	return true;
}

void GraphicScene::loadTexture(const std::string &imagePath)
{
	GLuint textureID;
	glGenTextures(1, &textureID);
	PngTexData texData;
	loadPng(imagePath.c_str(), texData);
	glBindTexture(GL_TEXTURE_2D, textureID);

	if(texData.alphaChannel)
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texData.width, texData.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, texData.data.get());
	else
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texData.width, texData.height, 0, GL_RGB, GL_UNSIGNED_BYTE, texData.data.get());

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glGenerateMipmap(GL_TEXTURE_2D);

	m_textures[imagePath] = textureID;
}



void GraphicScene::loadModels()
{
	// Caterpillar
	glm::vec4 caterpillarUV(0.0f, 0.0f, 200.0f / 512.0f, 50.0f / 512.0f);
	glm::vec4 tankFrontUV(0.0f, 50.0f / 512.0f, 150.0f / 512.0f, 100.0f / 512.0f);
	glm::vec4 tankBackUV(0.0f, 100.0f / 512.0f, 150.0f / 512.0f, 150.0f / 512.0f);
	glm::vec4 tankTopUV(0.0f, 150.0f / 512.0f, 200.0f / 512.0f, 300.0f / 512.0f);

	TexCoords caterpillarTexCoords;
	caterpillarTexCoords.rightUV = caterpillarUV;
	caterpillarTexCoords.leftUV = caterpillarTexCoords.rightUV;
	caterpillarTexCoords.frontUV = tankTopUV;
	caterpillarTexCoords.bottomUV = tankBackUV;
	caterpillarTexCoords.topUV = tankFrontUV;

	ModelPtr caterpillarModel = Model::Create();
	caterpillarModel->createModel36V(TEXTURES, glm::vec3(BLOCK_SIZE * 1.5f, BLOCK_SIZE * 2.0f, BLOCK_SIZE * 0.5f), caterpillarTexCoords);
	m_models["texCaterpillar"] = caterpillarModel;

	// Line
	ModelPtr lineModel = Model::Create();
	lineModel->createModel2V(glm::vec2(12800.0f, 0.0f));
	m_models["line"] = lineModel;

	// Plane
	ModelPtr planeModel = Model::Create();
	planeModel->createModel4V(TEXTURES, glm::vec2(BLOCK_SIZE, BLOCK_SIZE));
	m_models["texPlane"] = planeModel;

	// Cube
	ModelPtr simpCubeModel = Model::Create();
	simpCubeModel->createModel36V(SIMPLE, glm::vec3(100.0f, 100.0f, 100.0f));
	m_models["simpCube"] = simpCubeModel;

	ModelPtr normCubeModel = Model::Create();
	normCubeModel->createModel36V(NORMALS, glm::vec3(100.0f, 100.0f, 100.0f));
	m_models["normCube"] = normCubeModel;

	ModelPtr texCubeModel = Model::Create();
	texCubeModel->createModel36V(TEXTURES, glm::vec3(100.0f, 100.0f, 100.0f));
	m_models["texCube"] = texCubeModel;

	// Tower
	glm::vec4 towerSideUV(	0.0f,	  0.0f / 512.0f, 91.0f / 512.0f,  49.0f / 512.0f);
	glm::vec4 towerFrontUV(	0.0f,	 50.0f / 512.0f, 81.0f / 512.0f,  99.0f / 512.0f);
	glm::vec4 towerBackUV(	0.0f,	100.0f / 512.0f, 81.0f / 512.0f, 149.0f / 512.0f);
	glm::vec4 towerTopUV(	0.0f,	150.0f / 512.0f, 91.0f / 512.0f, 231.0f / 512.0f);

	glm::vec4 barretSideUV(	92.0f / 512.0f,	  0.0f,				111.0f / 512.0f, 149.0f / 512.0f);
	glm::vec4 barretFrontUV(92.0f / 512.0f,	150.0f / 512.0f,	111.0f / 512.0f, 169.0f / 512.0f);
	
	TexCoords towerTexCoords;
	towerTexCoords.rightUV = towerFrontUV;
	towerTexCoords.leftUV = towerBackUV;
	towerTexCoords.frontUV = towerTopUV;
	towerTexCoords.bottomUV = towerSideUV;
	towerTexCoords.topUV = towerSideUV;

	TexCoords barretTexCoords;
	barretTexCoords.rightUV = barretFrontUV;
	barretTexCoords.frontUV = barretSideUV;
	barretTexCoords.backUV = barretSideUV;
	barretTexCoords.bottomUV = barretSideUV;
	barretTexCoords.topUV = barretSideUV;

	ModelPtr towerModel = Model::Create();
	towerModel->createModel72V(TEXTURES, glm::vec3(BLOCK_SIZE * 0.94f, BLOCK_SIZE * 0.82f, BLOCK_SIZE * 0.5f), glm::vec3(BLOCK_SIZE * 1.5f, BLOCK_SIZE * 0.20f, BLOCK_SIZE * 0.20f), towerTexCoords, barretTexCoords);
	m_models["texTower"] = towerModel;

	// TexTree
	glm::vec4 trunkSideUV(0.0f, 0.0f, 150.0f / 512.0f, 20.0f / 512.0f);
	glm::vec4 crownSideUV(0.0f, 21.0f / 512.0f, 150.0f / 512.0f, 96.0f / 512.0f);
	glm::vec4 crownFrontUV(0.0f, 96.0f / 512.0f, 75.0f / 512.0f, 171.0f / 512.0f);

	TexCoords trunkTexCoords;
	trunkTexCoords.frontUV = trunkSideUV;
	trunkTexCoords.bottomUV = trunkSideUV;
	trunkTexCoords.topUV = trunkSideUV;
	trunkTexCoords.backUV = trunkSideUV;

	TexCoords crownTexCoords;
	crownTexCoords.rightUV = crownFrontUV;
	crownTexCoords.frontUV = crownSideUV;
	crownTexCoords.backUV = crownSideUV;
	crownTexCoords.bottomUV = crownSideUV;
	crownTexCoords.topUV = crownSideUV;
	crownTexCoords.leftUV = crownFrontUV;

	ModelPtr texTreeModel = Model::Create();
	texTreeModel->createModel72V(TEXTURES, glm::vec3(150.0f, 20.0f, 20.0f), glm::vec3(150.0f, 75.0f, 75.0f), trunkTexCoords, crownTexCoords);
	m_models["texTree"] = texTreeModel;

	// Fence
	ModelPtr normFenceModel = Model::Create();
	normFenceModel->createModel36V(NORMALS, glm::vec3(15.0f, 100.0f, 100.0f));
	m_models["normFence"] = normFenceModel;

	ModelPtr texFenceModel = Model::Create();
	texFenceModel->createModel36V(TEXTURES, glm::vec3(20.0f, 100.0f, 100.0f));
	m_models["texFence"] = texFenceModel;

	// NormBullet
	ModelPtr normBulletModel = Model::Create();
	normBulletModel->createModel36V(NORMALS, glm::vec3(10.0f, 15.0f, 10.0f));
	m_models["normBullet"] = normBulletModel;
}

void GraphicScene::loadShader(
	const std::string &shaderName,
	const std::string &vertexFileName,
	const std::string &fragmentFileName)
{
	ShaderPtr shader = Shader::Create(vertexFileName.c_str(),  fragmentFileName.c_str());
	m_shaders[shaderName] = shader;
}

void GraphicScene::addObject(const GraphicObjPtr& object)
{
	m_objects.push_back(object);
}

void GraphicScene::removeObject(const GraphicObjPtr& object)
{
	removeObject(object.get());
}

void GraphicScene::removeObject(GraphicObj* object)
{
	for (std::vector<GraphicObjPtr>::iterator	it = m_objects.begin(),
												end = m_objects.end();
												it != end;
												++it)
	{
		if ((*it).get() == object)
		{
			m_objects.erase(it);
			return;
		}
	}
}