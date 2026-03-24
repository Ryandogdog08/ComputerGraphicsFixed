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

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);
    //Enables opacity

    Gizmos::create(10000, 10000, 0, 0);

    aie::ImGui_Init(m_window, true);

    glfwSetCursorPosCallback(m_window, &Application::setMousePosition);

    m_shader.loadShader(aie::eShaderStage::VERTEX, "./Shaders/simple.vert");
    m_shader.loadShader(aie::eShaderStage::FRAGMENT, "./Shaders/simple.frag");

    m_phongShader.loadShader(aie::eShaderStage::VERTEX, "./Shaders/phong.vert");
    m_phongShader.loadShader(aie::eShaderStage::FRAGMENT, "./Shaders/phong.frag");

    m_normalMapShader.loadShader(aie::eShaderStage::VERTEX, "./Shaders/normalmap.vert");
    m_normalMapShader.loadShader(aie::eShaderStage::FRAGMENT, "./Shaders/normalmap.frag");

    m_normalColouredShader.loadShader(aie::eShaderStage::VERTEX, "./Shaders/normalColoured.vert");
    m_normalColouredShader.loadShader(aie::eShaderStage::FRAGMENT, "./Shaders/normalColoured.frag");

    m_negativeShader.loadShader(aie::eShaderStage::VERTEX, "./Shaders/negative.vert");
    m_negativeShader.loadShader(aie::eShaderStage::FRAGMENT, "./Shaders/negative.frag");

    m_warpedShader.loadShader(aie::eShaderStage::VERTEX, "./Shaders/warped.vert");
    m_warpedShader.loadShader(aie::eShaderStage::FRAGMENT, "./Shaders/warped.frag");

    m_heightScaleShader.loadShader(aie::eShaderStage::VERTEX, "./Shaders/heightScale.vert");
    m_heightScaleShader.loadShader(aie::eShaderStage::FRAGMENT, "./Shaders/heightScale.frag");

    m_mirageShader.loadShader(aie::eShaderStage::VERTEX, "./Shaders/mirage.vert");
    m_mirageShader.loadShader(aie::eShaderStage::FRAGMENT, "./Shaders/mirage.frag");

    m_phantomShader.loadShader(aie::eShaderStage::VERTEX, "./Shaders/phantom.vert");
    m_phantomShader.loadShader(aie::eShaderStage::FRAGMENT, "./Shaders/phantom.frag");

    m_stealthShader.loadShader(aie::eShaderStage::VERTEX, "./Shaders/stealth.vert");
    m_stealthShader.loadShader(aie::eShaderStage::FRAGMENT, "./Shaders/stealth.frag");

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

    if (m_normalColouredShader.link() == false) {
        printf("Normal Coloured Shader Error: %s\n", m_normalColouredShader.getLastError());
        return false;
    }

    if (m_negativeShader.link() == false) {
        printf("Negative Shader Error: %s\n", m_negativeShader.getLastError());
        return false;
    }

    if (m_warpedShader.link() == false) {
        printf("Warped Shader Error: %s\n", m_warpedShader.getLastError());
        return false;
    }

    if (m_heightScaleShader.link() == false) {
        printf("Height Scale Shader Error: %s\n", m_heightScaleShader.getLastError());
        return false;
    }

    if (m_mirageShader.link() == false) {
        printf("Mirage Shader Error: %s\n", m_mirageShader.getLastError());
        return false;
    }

    if (m_phantomShader.link() == false) {
        printf("Phantom Shader Error: %s\n", m_phantomShader.getLastError());
        return false;
    }

    if (m_stealthShader.link() == false) {
        printf("Stealth Shader Error: %s\n", m_stealthShader.getLastError());
        return false;
    }

    m_spearMesh.initialiseFromFile("soulspear/soulspear.obj");
    m_spearMesh.loadMaterial("soulspear/soulspear.mtl");

    m_scene = new Scene(&m_camera, glm::vec2(m_windowWidth, m_windowHeight), m_light, m_ambientLight);

    m_scene->addInstance(new Instance(glm::vec3(0, 0, 0), glm::vec3(0, 90, 0), glm::vec3(1, 1, 1), &m_spearMesh, &m_normalMapShader));
    m_scene->getPointLights().push_back(Light(vec3(0, 3, 5), vec3(1, 0, 0), 50));
    m_scene->getPointLights().push_back(Light(vec3(0, 3, -5), vec3(0, 1, 0), 50));

    /*for (int i = -4; i < 5; i++) {
    m_scene->addInstance(new Instance(glm::vec3(0,0,0+2.5*i),glm::vec3(0,90 - i*15,0),glm::vec3(1,1,1), &m_spearMesh, &m_normalMapShader));
    }*/

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

    if (m_scene->getShowGui()) {
        ImGui::Begin("Shader");
        if (ImGui::Button("Simple Shader")) {
            m_scene->setShaders(&m_shader);
            m_activeShader = 1;
        }
        if (ImGui::Button("Phong Shader")) {
            m_scene->setShaders(&m_phongShader);
            m_activeShader = 2;
        }
        if (ImGui::Button("Normal Map Shader")) {
            m_scene->setShaders(&m_normalMapShader);
            m_activeShader = 3;
        }
        if (ImGui::Button("Normal Coloured Shader")) {
            m_scene->setShaders(&m_normalColouredShader);
            m_activeShader = 4;
        }
        if (ImGui::Button("Negative Shader")) {
            m_scene->setShaders(&m_negativeShader);
            m_activeShader = 5;
        }
        if (ImGui::Button("Warped Shader")) {
            m_scene->setShaders(&m_warpedShader);
            m_activeShader = 6;
        }
        if (ImGui::Button("Stealth Shader")) {
            m_scene->setShaders(&m_stealthShader);
            m_activeShader = 7;
        }
        if (ImGui::Button("Height Scale Shader")) {
            m_scene->setShaders(&m_heightScaleShader);
            m_activeShader = 8;
            m_scene->setShaderScaling1(1);
        }
        if (ImGui::Button("Mirage Shader")) {
            m_scene->setShaders(&m_mirageShader);
            m_activeShader = 9;
            m_scene->setShaderScaling1(1);
            m_scene->setShaderScaling2(1);
        }
        if (ImGui::Button("Phantom Shader")) {
            m_scene->setShaders(&m_phantomShader);
            m_activeShader = 10;
            m_scene->setShaderScaling1(1);
            m_scene->setShaderScaling2(1);
        }
        ImGui::End();

        if (m_activeShader > 7) {
            ImGui::Begin("Shader Controls");

            if (m_activeShader == 8) {
                ImGui::DragFloat("Base Scale", m_scene->getShaderScaling1(), 0.1f, 0.1f, 10.0f);
            }
            if (m_activeShader == 9) {
                ImGui::DragFloat("Vanishing Distance", m_scene->getShaderScaling1(), 0.1f, 1.0f, 5.0f);
                ImGui::DragFloat("Distortion Power", m_scene->getShaderScaling2(), 0.1f, 0.0f, 10.0f);
            }
            if (m_activeShader == 10) {
                ImGui::DragFloat("Vanishing Height", m_scene->getShaderScaling1(), 0.1f, 0.0f, 30.0f);
                ImGui::DragFloat("Distortion Power", m_scene->getShaderScaling2(), 0.1f, 0.0f, 10.0f);
            }
            ImGui::End();
            m_scene->setShaderScaling1OnInstances();
            m_scene->setShaderScaling2OnInstances();
            m_scene->setShaderScaling3OnInstances();
        } //Shaders with controls 
    }


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
