#pragma once

#include <GLFW/glfw3.h>
#include "GraphicObj.h"
#include "PhysicalObj.h"
#include "GameObj.h"
#include "Tank.h"


class GameScene
{
public:
	GameScene();

	bool initialize();
	bool update(float dt);

	void createBulet(const glm::vec3 &position, float angle);

	void processKeyCallback(
		GLFWwindow* window,
		int key,
		int scancode,
		int action,
		int mode);

	void processMouseCallback(
		GLFWwindow*	window,
		double xpos,
		double ypos);

	void processScrollCallback(
		GLFWwindow* window,
		double xoffset,
		double yoffset);

	bool loadLevel(const std::string &name);

	void addObject(const GameObjPtr& object);
	void removeObject(const GameObjPtr& object);
	void removeObject(GameObj* object);

	void setEnemiesCount(int count) { m_enemiesCount = count; }
	int getEnemiesCount() { return m_enemiesCount; }

	bool getKey(int key) { return m_keys[key]; }
	const TankPtr& getTankPtr() { return m_tankTarget; }

private:
	bool m_keys[1024];
	float m_lastX;
	float m_lastY;
	bool m_firstMouse;
	bool m_isCameraFree;

	glm::vec3 m_cameraAnchor;
	glm::vec3 m_cameraTarget;

	TankPtr m_tankTarget;
	int m_enemiesCount;

	float time;
	int sec;
	
	std::vector<GameObjPtr> m_objects;
	std::vector<GameObjPtr> m_garbageCollector;
};

