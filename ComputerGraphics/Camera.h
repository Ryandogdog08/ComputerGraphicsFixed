#pragma once
#include "glad.h"
#include <GLFW/glfw3.h >
#include <glm/glm.hpp>
#include <glm/ext.hpp>

class Camera
{
public:
	Camera();
	glm::mat4 getViewMatrix();
	glm::mat4 getProjectionMatrix(float w, float h);
	glm::vec3 getPosition() { return m_position; }
	void update(float deltaTime, GLFWwindow* window);
	glm::vec2 getRotation() { return glm::vec2(m_theta, m_phi); }

protected:
	float m_theta;
	float m_phi;
	glm::vec3 m_position;
};

