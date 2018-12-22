#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include <memory>


using CameraPtr = std::shared_ptr<class Camera>;

enum Camera_Movement {
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT
};

class Camera
{
public:
	Camera(
		glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f),
		float yaw = -90.0f,
		float pitch = 0.0f,
		float zoom = 45.0f);

	Camera(
		float posX,
		float posY,
		float posZ,
		float upX,
		float upY,
		float upZ,
		float yaw,
		float pitch,
		float zoom = 45.0f);

	static CameraPtr Create(
		glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f),
		float yaw = -90.0f,
		float pitch = 0.0f,
		float zoom = 45.0f) { return std::make_shared<Camera>(position, up, yaw, pitch, zoom); }

	static CameraPtr Create(
		float posX,
		float posY,
		float posZ,
		float upX,
		float upY,
		float upZ,
		float yaw,
		float pitch,
		float zoom = 45.0f) { return std::make_shared<Camera>(posX, posY, posZ, upX, upY, upZ, yaw, pitch, zoom); }

	void updateCameraVectors();
	void updateTarget(const glm::vec3 &position, const glm::vec3 &target);

	glm::mat4 getFreeViewMatrix()	{ return glm::lookAt(m_position, m_position + m_front, m_up); }
	glm::mat4 getTargetViewMatrix() { return glm::lookAt(m_position, m_target, m_up); }

	glm::vec3 getPosition() { return m_position; }
	glm::vec3 getFront()	{ return m_front; }
	glm::vec3 getUp()		{ return m_up; }
	glm::vec3 getRight()	{ return m_right; }
	glm::vec3 getWorldUp()	{ return m_worldUp; }

	bool getIsFree() { return m_isFree; }
	
	float getYaw()	{ return m_yaw; }
	float getPitch(){ return m_pitch; }
	float getZoom()	{ return m_zoom; }

	void setPosition(glm::vec3 position) { m_position = position; }

	void setIsFree(bool isFree) { m_isFree = isFree; }

	void setYaw(float yaw)		{ m_yaw = yaw; }
	void setPitch(float pitch)	{ m_pitch = pitch; }
	void setZoom(float zoom)	{ m_zoom = zoom; }

private:
	glm::vec3 m_position;
	glm::vec3 m_front;
	glm::vec3 m_up;
	glm::vec3 m_right;
	glm::vec3 m_worldUp;
	glm::vec3 m_target;

	bool m_isFree;

	float m_yaw;
	float m_pitch;
	float m_zoom;
};

