#pragma once

#include "GameScene.h"
#include "PhysicalScene.h"
#include "GraphicScene.h"
#include "SoundManager.h"


class Game
{
public:
	Game();
	~Game();

	bool initialize();
	bool update();

	GameScene*	getGameScene() { return m_gameScene; }
	PhysicalScene*	getPhysicalScene() { return m_physicalScene; }
	GraphicScene*	getGraphicScene() { return m_graphicScene; }
	SoundManager*	getSoundManager() { return m_soundManager; }

	static Game* GetInstancePtr() { return s_instance; }

private:
	GameScene * m_gameScene;
	PhysicalScene*	m_physicalScene;
	GraphicScene*	m_graphicScene;
	SoundManager*	m_soundManager;

	float m_lastFrameTime;

	static Game* s_instance;
};