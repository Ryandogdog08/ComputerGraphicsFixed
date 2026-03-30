#include "Scene.h"
#include "Instance.h"
#include "imgui_glfw3.h"
#include <iostream>

Scene::Scene(Camera* camera, glm::vec2 windowSize, Light& light, glm::vec3 ambientLight)
{
	m_camera = camera;
	m_windowSize = windowSize;
	m_sunLight = light;
	m_ambientLight = ambientLight;

	m_targetSpawnPos = glm::vec3(0, 0, 0);
	m_targetSpawnRot = glm::vec3(0, 0, 0);
	m_targetSpawnScale = glm::vec3(1, 1, 1);
}

Scene::~Scene()
{
	for (auto it = m_instances.begin(); it != m_instances.end(); it++) {
		delete* it;
	}
}

void Scene::addInstance(Instance* instance)
{
	m_instances.push_back(instance);
}

void Scene::draw()
{
	if (m_showGui) {
		ImGui::Begin("Sunlight Settings");
		ImGui::DragFloat3("Sunlight Direction", &m_sunLight.direction[0], 0.1f, -1.0f, 1.0f);
		ImGui::ColorEdit3("Sunlight Colour", &m_sunLight.colour[0]);
		ImGui::End();

		ImGui::Begin("Object Spawner");
		if (ImGui::Button("Spawn Object on Player")) {
			Instance* firstInstance = *m_instances.begin();
			if (firstInstance != nullptr) {
				addInstance(new Instance(m_camera->getPosition(), glm::vec3(0, 90 - m_camera->getRotation().x, 0), glm::vec3(1, 1, 1), firstInstance->getMesh(), firstInstance->getShader()));
			}
		}
		if (ImGui::Button("Spawn Object Below Player")) {
			Instance* firstInstance = *m_instances.begin();
			if (firstInstance != nullptr) {
				addInstance(new Instance(glm::vec3(m_camera->getPosition().x, 0, m_camera->getPosition().z), glm::vec3(0, 90 - m_camera->getRotation().x, 0), glm::vec3(1, 1, 1), firstInstance->getMesh(), firstInstance->getShader()));
			}
		}
		if (ImGui::Button("Spawn Object at Position")) {
			Instance* firstInstance = *m_instances.begin();
			if (firstInstance != nullptr) {
				addInstance(new Instance(m_targetSpawnPos, m_targetSpawnRot, m_targetSpawnScale, firstInstance->getMesh(), firstInstance->getShader()));
			}
		}
		ImGui::NewLine();
		ImGui::DragFloat3("Spawn Position", (float*)&m_targetSpawnPos, 0.5f, -10.0f, 10.0f);
		ImGui::DragFloat3("Spawn Rotation", (float*)&m_targetSpawnRot, 1.0f, -180.0f, 180.0f);
		ImGui::DragFloat3("Spawn Scale", (float*)&m_targetSpawnScale, 0.1f, 0.1f, 10.0f);
		ImGui::End();
		
		ImGui::Begin("Point Light Controls");
		if (ImGui::Button("Spawn New Light") && m_pointLights.size() < MAX_LIGHTS) {
			m_pointLights.push_back(Light(glm::vec3(0, 0, 0), glm::vec3(0, 0, 0), 50));
		}
		ImGui::SameLine();
		if (ImGui::Button("Delete Latest Light")) {
			m_pointLights.pop_back();
		}
		ImGui::NewLine();
		for (int i = 1; i <= m_pointLights.size(); i++) {
			std::string posName = "Light " + std::to_string(i) + " Position";
			std::string colourName = "Light " + std::to_string(i) + " Colour";
			std::string intensityName = "Light " + std::to_string(i) + " Intensity";
			ImGui::DragFloat3(posName.c_str(), (float*)m_pointLights[i - 1].getDirection(), 0.5f, -10.0f, 10.0f);
			ImGui::ColorEdit3(colourName.c_str(), (float*)m_pointLights[i - 1].getColour());
			ImGui::DragFloat(intensityName.c_str(), (float*)m_pointLights[i - 1].getIntensity(), 1.0f, 0.0f, 250.0f);
			ImGui::NewLine();
		}
		ImGui::End();
	}

	ImGui::Begin("GUI Controls");
	if (ImGui::Button("Toggle GUI")) {
		m_showGui = !m_showGui;
	}
	ImGui::End();

	for (int i = 0; i < MAX_LIGHTS && i < m_pointLights.size(); i++) {
		m_pointLightPositions[i] = m_pointLights[i].direction;
		m_pointLightColours[i] = m_pointLights[i].colour;
		m_pointLights[i].updateColour();
	}

	for (auto it = m_instances.begin(); it != m_instances.end(); it++) {
		Instance* instance = *it;
		instance->draw(this);
	}
}

void Scene::setShaders(aie::ShaderProgram* _shader)
{
	for (auto it = m_instances.begin(); it != m_instances.end(); it++) {
		Instance* instance = *it;
		instance->setShader(_shader);
	}
}

void Scene::setShaderScaling1OnInstances()
{
	for (auto it = m_instances.begin(); it != m_instances.end(); it++) {
		Instance* instance = *it;
		instance->setShaderScaling1(m_shaderScaling1);
	}
}

void Scene::setShaderScaling2OnInstances()
{
	for (auto it = m_instances.begin(); it != m_instances.end(); it++) {
		Instance* instance = *it;
		instance->setShaderScaling2(m_shaderScaling2);
	}
}

void Scene::setShaderScaling3OnInstances()
{
	for (auto it = m_instances.begin(); it != m_instances.end(); it++) {
		Instance* instance = *it;
		instance->setShaderScaling3(m_shaderScaling3);
	}
}



