#include "stdafx.h"
#include "Camera.h"


Camera::Camera(
	glm::vec3 position,
	glm::vec3 up,
	float yaw,
	float pitch,
	float zoom)
	: m_position(position)
	, m_worldUp(up)
	, m_yaw(yaw)
	, m_pitch(pitch)
	, m_zoom(zoom)
	, m_target(0)
	, m_isFree(false)
{
	updateCameraVectors();
}

Camera::Camera(
	float posX,
	float posY,
	float posZ,
	float upX,
	float upY,
	float upZ,
	float yaw,
	float pitch,
	float zoom)
	: m_front(0.0f, 0.0f, -1.0f)
	, m_position(posX, posY, posZ)
	, m_worldUp(upX, upY, upZ)
	, m_yaw(yaw)
	, m_pitch(pitch)
	, m_target(0)
	, m_isFree(false)
{
	updateCameraVectors();
}

void Camera::updateCameraVectors()
{
	glm::vec3 front;
	front.x = cos(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
	front.y = sin(glm::radians(m_pitch));
	front.z = sin(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
	m_front = glm::normalize(front);
	m_right = glm::normalize(glm::cross(m_front, m_worldUp));
	m_up	= glm::normalize(glm::cross(m_right, m_front));
}

void Camera::updateTarget(const glm::vec3 &position, const glm::vec3 &target)
{
	m_position = position;
	m_target = target;
	m_up = glm::vec3(0.0f, 1.0f, 0.0f);
}