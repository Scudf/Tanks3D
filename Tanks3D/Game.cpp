#include "stdafx.h"
#include <glew/glew.h>
#include "Game.h"
#include <conio.h>
#include <Windows.h>


Game* Game::s_instance = nullptr;

Game::Game()
	: m_lastFrameTime(0.0f)
	, m_framesCount(0)
	, m_timer(0.0f)
{
	s_instance = this;
}


Game::~Game()
{
	delete m_soundManager;
	delete m_gameScene;
	delete m_physicalScene;
	delete m_graphicScene;
}

bool Game::initialize()
{
	m_graphicScene = new GraphicScene();
	if (!m_graphicScene->initialize())
		return false;

	m_physicalScene = new PhysicalScene();
	if (!m_physicalScene->initialize())
		return false;

	m_gameScene = new GameScene();
	if (!m_gameScene->initialize())
		return false;

	m_soundManager = new SoundManager();
	if (!m_soundManager->initialize())
		return false;

	m_lastFrameTime = (float)glfwGetTime();

	return true;
}

bool Game::update()
{
	float currentFrameTime = (float)glfwGetTime();
	float deltaTime = currentFrameTime - m_lastFrameTime;
	m_lastFrameTime = currentFrameTime;

	m_timer += deltaTime;
	//float fps = 1 / deltaTime;
	++m_framesCount;

	HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);

	if(m_framesCount < 24)
		SetConsoleTextAttribute(handle, FOREGROUND_RED);
	else
		SetConsoleTextAttribute(handle, FOREGROUND_GREEN);
		
	if (m_timer >= 1.0f)
	{
		std::cout << m_framesCount << '\n';
		m_timer = 0.0f;
		m_framesCount = 0;
	}
	if (!m_soundManager->update(deltaTime))
		return false;

	if (!m_gameScene->update(deltaTime))
		return false;

	if (!m_physicalScene->update(deltaTime))
		return false;

	if (!m_graphicScene->update(deltaTime))
		return false;

	return true;
}