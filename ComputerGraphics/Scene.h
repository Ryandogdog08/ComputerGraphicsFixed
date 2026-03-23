#pragma once
#include "Camera.h"
#include <list>
#include <vector>
#include <glm/glm.hpp>
#include <glm/ext.hpp>


const int MAX_LIGHTS = 4;
class Instance; 

struct Light {
	glm::vec3 direction;
	glm::vec3 colour;

	Light() {};
	Light(glm::vec3 pos, glm::vec3 col, float intensity)
	{
		direction = pos;
		colour = col * intensity;
	}
};

class Scene
{
public:
	Scene(Camera* camera, glm::vec2 windowSize, Light& light, glm::vec3 ambientLight);
	~Scene();
	void addInstance(Instance* instance);
	void draw();

	Camera* getCamera() { return m_camera; }
	glm::vec2 getWindowSize() { return m_windowSize; }
	Light getLight() { return m_sunLight; }
	glm::vec3 getAmbientLight() { return m_ambientLight; }

	int getNumLights() { return (int)m_pointLights.size(); }
	glm::vec3* getPointLightPositions() { return &m_pointLightPositions[0]; }
	glm::vec3* getPointLightColours() { return &m_pointLightColours[0]; }
	std::vector<Light>& getPointLights() { return m_pointLights; }

protected:

	Camera* m_camera;
	glm::vec2 m_windowSize;
	Light m_sunLight;
	std::vector<Light> m_pointLights;
	glm::vec3 m_ambientLight;
	std::list<Instance*> m_instances;

	glm::vec3 m_pointLightPositions[MAX_LIGHTS];
	glm::vec3 m_pointLightColours[MAX_LIGHTS];

	glm::vec3 m_targetSpawnPos;
	glm::vec3 m_targetSpawnRot;
	glm::vec3 m_targetSpawnScale;

};

