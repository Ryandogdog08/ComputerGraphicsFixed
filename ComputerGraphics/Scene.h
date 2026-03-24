#pragma once
#include "Camera.h"
#include <list>
#include <vector>
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include "Shader.h"


const int MAX_LIGHTS = 4;
class Instance; 

struct Light {
	glm::vec3 direction;
	glm::vec3 colour;
	glm::vec3 colourValue;
	float m_intensity;

	Light() {};
	Light(glm::vec3 pos, glm::vec3 col, float intensity)
	{
		direction = pos;
		m_intensity = intensity;
		colourValue = col;
		updateColour();
	}
	glm::vec3* getDirection() { return &direction; }
	glm::vec3* getColour() { return &colourValue; }
	float* getIntensity() { return &m_intensity; }

	void updateColour() { colour = colourValue * m_intensity; }
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

	void setShaders(aie::ShaderProgram* _shader);
	void setShaderScaling1OnInstances();
	void setShaderScaling2OnInstances();
	void setShaderScaling3OnInstances();

	void setShaderScaling1(float _value) { m_shaderScaling1 = _value; }
	void setShaderScaling2(float _value) { m_shaderScaling2 = _value; }
	void setShaderScaling3(float _value) { m_shaderScaling3 = _value; }

	float* getShaderScaling1() { return &m_shaderScaling1; }
	float* getShaderScaling2() { return &m_shaderScaling2; }
	float* getShaderScaling3() { return &m_shaderScaling3; }

	bool getShowGui() { return m_showGui; }

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

	float m_shaderScaling1;
	float m_shaderScaling2;
	float m_shaderScaling3;

	bool m_showGui = true;


};

