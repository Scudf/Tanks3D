#include "stdafx.h"
#include "SoundManager.h"
#include "squall.h"
#include "Game.h"


SoundManager::SoundManager()
{
}


SoundManager::~SoundManager()
{
}

bool SoundManager::initialize()
{
	if (SQUALL_Init(0) < 0)
	{
		SQUALL_Free();
		return false;
	}

	m_musicSample = SQUALL_Sample_LoadFile((char*)"sounds/main.mp3", 1, 0);
	m_musicChannel = SQUALL_Sample_Play(m_musicSample, 1, 0, 0);
	SQUALL_Channel_Start(m_musicChannel);
	SQUALL_Channel_SetVolume(m_musicChannel, 85);
	SQUALL_Channel_SetVolume(m_soundChannel, 100);

	return true;
}

bool SoundManager::update(float dt)
{
	glm::vec3 tPos = Game::GetInstancePtr()->getGameScene()->getTankPtr()->getGraphicObject()->getPosition() / 100.0f;
	float tfPos[3] = { tPos.x, tPos.y, tPos.z };
	SQUALL_Listener_SetPosition(tfPos);

	return true;
}

void SoundManager::playSound3D(std::string sSound, glm::vec3 pos)
{
	pos = pos / 100.0f;
	m_soundSample = SQUALL_Sample_LoadFile((char*)sSound.c_str(), 1, 0);
	float afPos[3] = { pos.x, pos.y, pos.z };
	m_soundChannel = SQUALL_Sample_Play3D(m_soundSample, 0, 0, 0, afPos, 0);
	SQUALL_Channel_Start(m_soundChannel);
}
