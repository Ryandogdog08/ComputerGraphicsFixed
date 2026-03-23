#pragma once
#include "glad.h"
#include <GLFW/glfw3.h >
#include "Gizmos.h"
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include "Shader.h"
#include "Mesh.h"
#include "Camera.h"
#include "Texture.h"
#include "Scene.h"


using glm::vec3;
using glm::vec4;
using glm::mat4;
using aie::Gizmos;



class Application
{
public:
	Application();
	~Application();

	bool startup();
	bool update();
	void draw();
	void shutdown();

	static Application* get() { return s_instance; }
	glm::vec2 getMousePosition() { return m_mousePosition; }
	glm::vec2 getMouseDelta() { return m_mousePosition - m_lastMousePosition; }

	static void setMousePosition(GLFWwindow* window, double x, double y);

protected:

	GLFWwindow* m_window;
	const int m_windowHeight = 720;
	const int m_windowWidth = 1280;

	aie::ShaderProgram m_shader;
	aie::ShaderProgram m_phongShader;
	aie::ShaderProgram m_normalMapShader; 
	aie::ShaderProgram m_normalColouredShader; 
	aie::ShaderProgram m_negativeShader; 
	aie::ShaderProgram m_warpedShader; 
	aie::ShaderProgram m_heightScaleShader; 

	
	Mesh m_spearMesh;
	Scene* m_scene;

	static Application* s_instance;
	glm::vec2 m_mousePosition;
	glm::vec2 m_lastMousePosition;

	Camera m_camera;

	
	Light m_light;
	glm::vec3 m_ambientLight;

};

