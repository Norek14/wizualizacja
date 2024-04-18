#pragma once

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

enum class CameraType
{
	STATIONARY = 0, LOOK_AT = 1, FOLLOW = 2
};

class Camera
{
public:
	inline Camera(glm::vec3 camera_pos) : m_camera_pos(camera_pos) {}

	inline glm::mat4& GetOscillatingViewMatrix(float time, const glm::vec3& pos)
	{
		const float radius = 10.0f;
		float camX = sin(time) * radius;
		float camZ = cos(time) * radius;
		m_view_matrix = glm::lookAt(pos + glm::vec3(camX, 0.0, camZ), m_camera_target, m_camera_up);
		return m_view_matrix;
	}

	inline glm::mat4& GetViewMatrix(const glm::vec3& pos)
	{
		m_view_matrix = glm::lookAt(pos, m_camera_target, m_camera_up);
		return m_view_matrix;
	}

	inline glm::mat4& GetViewMatrix(const glm::vec3& pos, const glm::vec3& custom_target)
	{
		m_view_matrix = glm::lookAt(pos, custom_target, m_camera_up);
		return m_view_matrix;
	}
private:
	glm::vec3 m_camera_up = glm::vec3(0.0f, 1.0f, 0.0f);
	glm::vec3 m_camera_pos;
	glm::vec3 m_camera_target = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::mat4 m_view_matrix = glm::mat4(1.0f);

};