#include "stdafx.h"
#include <GL/glew.h>
#include "GameScene.h"
#include "Game.h"
#include "GraphicScene.h"
#include "Tank.h"
#include "TankPlayer.h"
#include "TankBot.h"
#include <tinyxml2.h>
#include "Bullet.h"


void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	Game::GetInstancePtr()->getGameScene()->processKeyCallback(window, key, scancode, action, mode);
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	Game::GetInstancePtr()->getGameScene()->processMouseCallback(window, xpos, ypos);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	Game::GetInstancePtr()->getGameScene()->processScrollCallback(window, xoffset, yoffset);
}

GameScene::GameScene()
	: m_firstMouse(true)
	, m_isCameraFree(false)
	, m_cameraAnchor(0)
	, m_cameraTarget(0)
	, m_enemiesCount(0)
{
	GraphicScene*	gs = Game::GetInstancePtr()->getGraphicScene();
	m_lastX = gs->getWidth() / 2;
	m_lastY = gs->getHeight() / 2;
}

bool GameScene::initialize()
{
	GLFWwindow* window = Game::GetInstancePtr()->getGraphicScene()->getWindow();
	GraphicScene*	gs = Game::GetInstancePtr()->getGraphicScene();
	PhysicalScene*	ps = Game::GetInstancePtr()->getPhysicalScene();

	glfwSetKeyCallback(window, key_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	/*GraphicObjPtr xAxis = GraphicObj::Create(gs->getModel("line"), gs->getGLShader("simpleShader"));
	xAxis->setColor(glm::vec3(1.0f, 0.0f, 0.0f));
	xAxis->setScale(glm::vec3(2.0f, 0.0f, 0.0f));
	gs->addObject(xAxis);

	GraphicObjPtr yAxis = GraphicObj::Create(gs->getModel("line"), gs->getGLShader("simpleShader"));
	yAxis->setColor(glm::vec3(0.0f, 1.0f, 0.0f));
	yAxis->setRotation(glm::vec3(0.0f, 0.0f, glm::radians(90.0f)));
	yAxis->setScale(glm::vec3(2.0f, 0.0f, 0.0f));
	gs->addObject(yAxis);

	GraphicObjPtr zAxis = GraphicObj::Create(gs->getModel("line"), gs->getGLShader("simpleShader"));
	zAxis->setColor(glm::vec3(0.0f, 0.0f, 1.0f));
	zAxis->setRotation(glm::vec3(0.0f, glm::radians(-90.0f), 0.0f));
	zAxis->setScale(glm::vec3(2.0f, 0.0f, 0.0f));
	gs->addObject(zAxis);*/

	BulletPtr bull = Bullet::Create(glm::vec3(200.0f, 200.0f, 100.f), 0.0f);
	addObject(bull);

	loadLevel("levels/level0.xml");

	return true;
}

bool GameScene::update(float dt)
{
	m_garbageCollector.clear();

	CameraPtr camera = Game::GetInstancePtr()->getGraphicScene()->getCamera();
	camera->setIsFree(m_isCameraFree);

	float velocity = CAMERA_SPEED * dt;

	if (m_isCameraFree)
	{
		if (m_keys[GLFW_KEY_UP])
			camera->setPosition(camera->getPosition() + camera->getFront() * velocity);
		if (m_keys[GLFW_KEY_DOWN])
			camera->setPosition(camera->getPosition() - camera->getFront() * velocity);
		if (m_keys[GLFW_KEY_LEFT])
			camera->setPosition(camera->getPosition() - camera->getRight() * velocity);
		if (m_keys[GLFW_KEY_RIGHT])
			camera->setPosition(camera->getPosition() + camera->getRight() * velocity);
	}
	else
	{
		m_cameraAnchor = glm::vec3(ConvertMetersToPxls(m_tankTarget->getB2Body()->GetPosition().x), ConvertMetersToPxls(m_tankTarget->getB2Body()->GetPosition().y) - 1700.0f, 1700.0f);
		m_cameraTarget = glm::vec3(ConvertMetersToPxls(m_tankTarget->getB2Body()->GetPosition().x), ConvertMetersToPxls(m_tankTarget->getB2Body()->GetPosition().y), 0.0f);
		camera->updateTarget(m_cameraAnchor, m_cameraTarget);
	}

	std::vector<GameObjPtr> temp = m_objects;
	for (GameObjPtr object : temp)
		object->update(dt);

	if (m_enemiesCount <= 0 || m_tankTarget->getHealth() <= 0)
		return false;

	return true;
}

void GameScene::createBulet(const glm::vec3 &position, float angle)
{
	BulletPtr bullet = Bullet::Create(position, angle);
	bullet->getB2Body()->ApplyLinearImpulse(ConvertPxlsToMeters(-sin(angle) * 2200.0f * bullet->getB2Body()->GetMass(), cos(angle) * 2200.0f * bullet->getB2Body()->GetMass()), bullet->getB2Body()->GetWorldCenter(), true);
	addObject(bullet);
}

void GameScene::processKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
	if (key >= 0 && key < 1024)
	{
		if (action == GLFW_PRESS)
			m_keys[key] = true;
		else if (action == GLFW_RELEASE)
			m_keys[key] = false;

		if (!m_keys[GLFW_KEY_C] && key == GLFW_KEY_C)
			m_isCameraFree = !m_isCameraFree;
	}
}

void GameScene::processMouseCallback(GLFWwindow* window, double xpos, double ypos)
{
	if (m_isCameraFree)
	{
		CameraPtr camera = Game::GetInstancePtr()->getGraphicScene()->getCamera();

		if (m_firstMouse)
		{
			m_lastX = (float)xpos;
			m_lastY = (float)ypos;
			m_firstMouse = false;
		}

		float xoffset = (float)xpos - m_lastX;
		float yoffset = m_lastY - (float)ypos;

		m_lastX = (float)xpos;
		m_lastY = (float)ypos;

		xoffset *= MOUSE_SENSITIVTY;
		yoffset *= MOUSE_SENSITIVTY;

		camera->setYaw(camera->getYaw() + xoffset);
		camera->setPitch(camera->getPitch() + yoffset);

		if (camera->getPitch() > 89.0f)
			camera->setPitch(89.0f);
		if (camera->getPitch() < -89.0f)
			camera->setPitch(-89.0f);

		camera->updateCameraVectors();
	}
	else
		m_firstMouse = true;
}

void GameScene::processScrollCallback(GLFWwindow* window, double xoffset, double yoffset)
{
	CameraPtr camera = Game::GetInstancePtr()->getGraphicScene()->getCamera();

	if (camera->getZoom() >= 1.0f && camera->getZoom() <= 45.0f)
		camera->setZoom(camera->getZoom() - (float)yoffset);
	if (camera->getZoom() <= 1.0f)
		camera->setZoom(1.0f);
	if (camera->getZoom() >= 45.0f)
		camera->setZoom(45.0f);
}

bool GameScene::loadLevel(const std::string &name)
{
	GraphicScene*	gs = Game::GetInstancePtr()->getGraphicScene();
	PhysicalScene*	ps = Game::GetInstancePtr()->getPhysicalScene();

	tinyxml2::XMLDocument doc;
	tinyxml2::XMLError loadError = doc.LoadFile(name.c_str());
	if (tinyxml2::XML_SUCCESS != loadError)
	{
		//	std::cout << "File '%s' loading error - XMLError: %d!", name.c_str(), (S32)loadError);
		return false;
	}

	tinyxml2::XMLNode* rootNode = doc.FirstChild();
	if (!rootNode)
	{
		//BFG_ERROR("File '%s' loading error - empty root node!", fileName.c_str());
		return false;
	}

	rootNode = rootNode->NextSibling();
	if (!rootNode)
	{
		//BFG_ERROR("File '%s' loading error - empty root node children!", fileName.c_str());
		return false;
	}

	tinyxml2::XMLElement* rootNodeElement = rootNode->ToElement();

	float width = rootNodeElement->FloatAttribute("width");
	float height = rootNodeElement->FloatAttribute("height");

	ps->createBounds(width, height);

	GraphicObjPtr ground = GraphicObj::Create(gs->getModel("texPlane"), gs->getGLShader("texturesShader"));
	ground->setScale(glm::vec3(width, height, 1.0f));
	ground->setMaterialTexture(gs->getGLTexture("textures/Ground_Diffuse.png"), gs->getGLTexture("textures/Ground_Specular.png"));
	ground->setColor(glm::vec3(0.2f, 0.2f, 0.2f));
	gs->addObject(ground);

	tinyxml2::XMLNode* levelChildNode = rootNode->FirstChild();
	while (levelChildNode)
	{
		tinyxml2::XMLElement* levelChildElement = levelChildNode->ToElement();

		if (!levelChildElement)
		{
			levelChildNode = levelChildNode->NextSibling();
			continue;
		}

		if (strcmp(levelChildElement->Name(), "TankPlayer") == 0)
		{
			float r = levelChildElement->FloatAttribute("r");
			float c = levelChildElement->FloatAttribute("c");
			TankPlayerPtr tankPlayer = TankPlayer::Create(glm::vec3(c * BLOCK_SIZE + BLOCK_SIZE / 2, r * BLOCK_SIZE + BLOCK_SIZE / 2, 26.0f));
			addObject(tankPlayer);

			m_tankTarget = tankPlayer;
		}

		if (strcmp(levelChildElement->Name(), "TankBot") == 0)
		{
			float r = levelChildElement->FloatAttribute("r");
			float c = levelChildElement->FloatAttribute("c");
			TankBotPtr tankBot = TankBot::Create(glm::vec3(c * BLOCK_SIZE + BLOCK_SIZE / 2, r * BLOCK_SIZE + BLOCK_SIZE / 2, 26.0f));
			addObject(tankBot);
			++m_enemiesCount;
		}

		if (strcmp(levelChildElement->Name(), "Rock") == 0)
		{
			float r = levelChildElement->FloatAttribute("r");
			float c = levelChildElement->FloatAttribute("c");

			PhysicalObjPtr rock = PhysicalObj::Create(gs->getModel("texCube"), gs->getGLShader("texturesShader"), glm::vec3(c * BLOCK_SIZE + BLOCK_SIZE / 2, r * BLOCK_SIZE + BLOCK_SIZE / 2, 51.0f), glm::vec2(100.0f, 100.0f), true);
			rock->getGraphicObject()->setMaterialTexture(gs->getGLTexture("textures/Fence_Diffuse.png"), gs->getGLTexture("textures/Fence_Specular.png"));
			ps->addObject(rock);
		}

		if (strcmp(levelChildElement->Name(), "Fence") == 0)
		{
			float r = levelChildElement->FloatAttribute("r");
			float c = levelChildElement->FloatAttribute("c");
			float angle = levelChildElement->FloatAttribute("angle");

			PhysicalObjPtr fence = PhysicalObj::Create(gs->getModel("texFence"), gs->getGLShader("texturesShader"), glm::vec3(c * BLOCK_SIZE + BLOCK_SIZE / 2, r * BLOCK_SIZE + BLOCK_SIZE / 2, 51.0f), glm::vec2(10.0f, 100.0f), true);
			fence->getGraphicObject()->setMaterialTexture(gs->getGLTexture("textures/Fence_Diffuse.png"), gs->getGLTexture("textures/Fence_Specular.png"));
			fence->getB2Body()->SetTransform(fence->getB2Body()->GetPosition(), glm::radians(angle));
			ps->addObject(fence);
		}

		if (strcmp(levelChildElement->Name(), "Tree") == 0)
		{
			float r = levelChildElement->FloatAttribute("r");
			float c = levelChildElement->FloatAttribute("c");

			PhysicalObjPtr tree = PhysicalObj::Create(gs->getModel("texTree"), gs->getGLShader("texturesShader"), glm::vec3(c * BLOCK_SIZE + BLOCK_SIZE / 2, r * BLOCK_SIZE + BLOCK_SIZE / 2, 76.0f), glm::vec2(20.0f, 20.0f), true);
			tree->getGraphicObject()->setMaterialTexture(gs->getGLTexture("textures/Tree_Diffuse.png"), gs->getGLTexture("textures/Tree_Specular.png"));
			tree->getGraphicObject()->setRotation(glm::vec3(0.0f, glm::radians(-90.0f), 0.0f));
			ps->addObject(tree);
		}

		if (strcmp(levelChildElement->Name(), "Box") == 0)
		{
			float r = levelChildElement->FloatAttribute("r");
			float c = levelChildElement->FloatAttribute("c");

			GameObjPtr box = GameObj::Create(gs->getModel("texCube"), gs->getGLShader("texturesShader"), glm::vec3(c * BLOCK_SIZE + BLOCK_SIZE / 2, r * BLOCK_SIZE + BLOCK_SIZE / 2, 51.0f), glm::vec2(100.0f, 100.0f));
			box->getGraphicObject()->setMaterialTexture(gs->getGLTexture("textures/Box_Diffuse.png"), gs->getGLTexture("textures/Box_Specular.png"));
			box->getB2Body()->GetFixtureList()->SetRestitution(0.2f);
			box->getB2Body()->GetFixtureList()->SetDensity(0.2f);
			box->getB2Body()->SetLinearDamping(3);
			box->getB2Body()->SetAngularDamping(3);
			box->getB2Body()->ResetMassData();
			addObject(box);
		}
		//BFString key = levelChildElement->Attribute("key");

		levelChildNode = levelChildNode->NextSibling();
	}

	return true;
}

void GameScene::addObject(const GameObjPtr& object)
{
	PhysicalScene* ps = Game::GetInstancePtr()->getPhysicalScene();

	m_objects.push_back(object);

	ps->addObject(std::static_pointer_cast<PhysicalObj>(object));
}

void GameScene::removeObject(const GameObjPtr& object)
{
	PhysicalScene* ps = Game::GetInstancePtr()->getPhysicalScene();

	removeObject(object.get());
}

void GameScene::removeObject(GameObj* object)
{
	PhysicalScene* ps = Game::GetInstancePtr()->getPhysicalScene();

	for (std::vector<GameObjPtr>::iterator	it = m_objects.begin(),
		end = m_objects.end();
		it != end;
		++it)
	{
		if ((*it).get() == object)
		{
			m_garbageCollector.push_back(*it);
			m_objects.erase(it);
			ps->removeObject(object);
			return;
		}
	}
}