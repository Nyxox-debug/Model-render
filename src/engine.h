#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <memory>

class Mesh;
class Shader;
class Model;

struct GLFWwindow;

class Engine {
private:
    GLFWwindow *window = nullptr;
    bool running = false;
    std::unique_ptr<Model> model;
    std::unique_ptr<Shader> shader;

    // Camera and Input
    void processInput();
    glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
    glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
    glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
    float deltaTime = 0.0f;
    float lastFrame = 0.0f;

    // Mouse
    static void mouse_callback(GLFWwindow *window, double xpos, double ypos);
    float yaw = -90.0f;
    float pitch = 0.0f;
    float lastX = 400, lastY = 300;
    bool firstMouse = true;

public:
    Engine();
    ~Engine();

    bool init();
    void run();
    void shutdown();
};
