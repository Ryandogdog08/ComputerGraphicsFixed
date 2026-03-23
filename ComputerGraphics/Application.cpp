#include "Application.h"
#include "Mesh.h"
#include "Instance.h"
#include "imgui_glfw3.h"


Application* Application::s_instance;

Application::Application()
{
}

Application::~Application()
{

}

bool Application::startup()
{
    s_instance = this;

    if (glfwInit() == false) {
        return false;
    }

    m_window = glfwCreateWindow(m_windowWidth, m_windowHeight, "Computer Graphics", nullptr, nullptr);

    if (m_window == nullptr) {
        shutdown();
        return false;
    }

    glfwMakeContextCurrent(m_window);

    if (!gladLoadGL()) {
        shutdown();
        return false;
    }

    glClearColor(0.05f, 0.25f, 0.25f, 1);
    glEnable(GL_DEPTH_TEST);
    Light light;
    light.colour = { 1,1,1 };
    m_ambientLight = { 0.25f, 0.25f, 0.25f };

    glm::mat4 spearTransform = {
        1,0,0,0,
        0,1,0,0,
        0,0,1,0,
        0,0,0,1
    };


    Gizmos::create(10000, 10000, 0, 0);

    aie::ImGui_Init(m_window, true);

    glfwSetCursorPosCallback(m_window, &Application::setMousePosition);

    m_shader.loadShader(aie::eShaderStage::VERTEX, "./Shaders/simple.vert");
    m_shader.loadShader(aie::eShaderStage::FRAGMENT, "./Shaders/simple.frag");

    m_phongShader.loadShader(aie::eShaderStage::VERTEX, "./Shaders/phong.vert");
    m_phongShader.loadShader(aie::eShaderStage::FRAGMENT, "./Shaders/phong.frag");

    m_normalMapShader.loadShader(aie::eShaderStage::VERTEX, "./Shaders/normalmap.vert");
    m_normalMapShader.loadShader(aie::eShaderStage::FRAGMENT, "./Shaders/normalmap.frag");

    if (m_shader.link() == false) {
        printf("Simple Shader Error: %s\n", m_shader.getLastError());
        return false;
    }

    if (m_phongShader.link() == false) {
        printf("PHONG Shader Error: %s\n", m_phongShader.getLastError());
        return false;
    }

    if (m_normalMapShader.link() == false) {
        printf("Normal Map Shader Error: %s\n", m_normalMapShader.getLastError());
        return false;
    }

    m_spearMesh.initialiseFromFile("soulspear/soulspear.obj");
    m_spearMesh.loadMaterial("soulspear/soulspear.mtl");

    m_scene = new Scene(&m_camera, glm::vec2(m_windowWidth, m_windowHeight), m_light, m_ambientLight);

    //m_scene->addInstance(new Instance(glm::vec3(0, 0, 0), glm::vec3(0, 90, 0), glm::vec3(1, 1, 1), &m_spearMesh, &m_normalMapShader));
    m_scene->getPointLights().push_back(Light(vec3(0, 3, 5), vec3(1, 0, 0), 50));
    m_scene->getPointLights().push_back(Light(vec3(0, 3, -5), vec3(0, 1, 0), 50));

    for (int i = -4; i < 5; i++) {
    m_scene->addInstance(new Instance(glm::vec3(0,0,0+2.5*i),glm::vec3(0,90 - i*15,0),glm::vec3(1,1,1), &m_spearMesh, &m_normalMapShader));
    }
    //Test

	return true;
}

bool Application::update()
{
    aie::ImGui_NewFrame();
    
    m_camera.update(0.1f, m_window);
    m_lastMousePosition = m_mousePosition;
    

    glfwSwapBuffers(m_window);
    glfwPollEvents();
    return (glfwWindowShouldClose(m_window) == false && glfwGetKey(m_window, GLFW_KEY_ESCAPE) != GLFW_PRESS);
}


void Application::draw()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::mat4 pv = m_camera.getProjectionMatrix(m_windowWidth, m_windowHeight) * m_camera.getViewMatrix();

    Gizmos::clear();
    Gizmos::addTransform(glm::mat4(1));

    vec4 white(1);
    vec4 black(0, 0, 0, 1);

    for (int i = 0; i < 21; i++) {
        Gizmos::addLine(vec3(-10 + i, 0, 10), vec3(-10 + i, 0, -10), i == 10 ? white : black);

        Gizmos::addLine(vec3(10, 0, -10 + i), vec3(-10, 0, -10 + i), i == 10 ? white : black);
    }
    Gizmos::draw(pv);

    m_scene->draw();
    ImGui::Render();

   
}

void Application::shutdown()
{
    aie::ImGui_Shutdown();
    Gizmos::destroy();
    delete m_scene;
    glfwDestroyWindow(m_window);
    glfwTerminate();
}

void Application::setMousePosition(GLFWwindow* window, double x, double y)
{
    s_instance->m_mousePosition.x = (float)x;
    s_instance->m_mousePosition.y = (float)y;
}
