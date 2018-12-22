#pragma once

#include <string>
#include <glew/glew.h>
#include <glm/glm.hpp>


class SoundManager
{
public:
	SoundManager();
	~SoundManager();

	bool initialize();
	bool update(float dt);

	void playSound3D(std::string sSound, glm::vec3 pos);

private:

	int m_musicSample;
	int m_musicChannel;

	int m_soundSample;
	int m_soundChannel;
};

