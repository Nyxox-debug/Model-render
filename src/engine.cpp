#include "engine.h"
#include "glm/ext/matrix_clip_space.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "model/mesh.h"
#include "model/model.h"
#include "shader/shader.h"
#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <iostream>
#include <memory>

Engine::Engine() = default;
Engine::~Engine() = default;

void Engine::processInput() {
  float currentFrame = glfwGetTime();
  deltaTime = currentFrame - lastFrame;
  lastFrame = currentFrame;

  float cameraSpeed = 2.5f * deltaTime;

  if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    cameraPos += cameraSpeed * cameraFront;
  if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    cameraPos -= cameraSpeed * cameraFront;
  if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    cameraPos -=
        glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
  if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    cameraPos +=
        glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;

  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    glfwSetWindowShouldClose(window, true);
}

void Engine::mouse_callback(GLFWwindow *window, double xpos, double ypos) {
  Engine *eng = static_cast<Engine *>(glfwGetWindowUserPointer(window));

  if (eng->firstMouse) {
    eng->lastX = xpos;
    eng->lastY = ypos;
    eng->firstMouse = false;
  }

  float xoffset = xpos - eng->lastX;
  float yoffset = eng->lastY - ypos; // reversed since y-coords go bottom to top
  eng->lastX = xpos;
  eng->lastY = ypos;

  float sensitivity = 0.1f;
  xoffset *= sensitivity;
  yoffset *= sensitivity;

  eng->yaw += xoffset;
  eng->pitch += yoffset;

  if (eng->pitch > 89.0f)
    eng->pitch = 89.0f;
  if (eng->pitch < -89.0f)
    eng->pitch = -89.0f;

  glm::vec3 front;
  front.x = cos(glm::radians(eng->yaw)) * cos(glm::radians(eng->pitch));
  front.y = sin(glm::radians(eng->pitch));
  front.z = sin(glm::radians(eng->yaw)) * cos(glm::radians(eng->pitch));
  eng->cameraFront = glm::normalize(front);
}

// Forgot to add framebuffer_size_callback
void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
  glViewport(0, 0, width, height); // Fixes “subwindow” effect on resizing
}

bool Engine::init() {
  if (!glfwInit()) {
    std::cerr << "Failed to initialize GLFW\n";
    return false;
  }

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  window = glfwCreateWindow(800, 600, "Engine Window", nullptr, nullptr);
  if (!window) {
    std::cerr << "Failed to create window\n";
    glfwTerminate();
    return false;
  }

  glfwMakeContextCurrent(window);

  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    std::cerr << "Failed to initialize GLAD\n";
    return false;
  }

  glEnable(GL_DEPTH_TEST);

  int width, height;
  glfwGetFramebufferSize(window, &width, &height);
  glViewport(0, 0, width, height);

  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

  glfwSetWindowUserPointer(window, this);
  glfwSetCursorPosCallback(window, Engine::mouse_callback);
  glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED); // hide cursor

  // NOTE: Square
  // std::vector<float> vertices = {0.5f,  0.5f,  0.0f, 0.5f,  -0.5f, 0.0f,
  //                                -0.5f, -0.5f, 0.0f, -0.5f, 0.5f,  0.0f};
  // std::vector<unsigned int> indices = {0, 1, 3, 1, 2, 3};

  // NOTE: Cube
  std::vector<float> vertices = {
      // x,     y,     z
      -0.5f, -0.5f, -0.5f, // 0
      0.5f,  -0.5f, -0.5f, // 1
      0.5f,  0.5f,  -0.5f, // 2
      -0.5f, 0.5f,  -0.5f, // 3
      -0.5f, -0.5f, 0.5f,  // 4
      0.5f,  -0.5f, 0.5f,  // 5
      0.5f,  0.5f,  0.5f,  // 6
      -0.5f, 0.5f,  0.5f   // 7
  };

  // 12 triangles (36 indices)
  std::vector<unsigned int> indices = {// back face
                                       0, 1, 2, 2, 3, 0,
                                       // front face
                                       4, 5, 6, 6, 7, 4,
                                       // left face
                                       4, 0, 3, 3, 7, 4,
                                       // right face
                                       1, 5, 6, 6, 2, 1,
                                       // bottom face
                                       4, 5, 1, 1, 0, 4,
                                       // top face
                                       3, 2, 6, 6, 7, 3};

  std::vector<std::unique_ptr<Mesh>> meshes;
  meshes.push_back(std::make_unique<Mesh>(vertices, indices));
  model = std::make_unique<Model>(std::move(meshes));

  // To place model origin at center
  model->transform.position = glm::vec3(0.0f);
  model->transform.scale = glm::vec3(1.0f);
  model->transform.rotation = glm::vec3(0.0f);

  shader = std::make_unique<Shader>("../res/shaders/def.vert",
                                    "../res/shaders/def.frag");

  running = true;
  return true;
}

void Engine::run() {
  if (!running)
    return;

  while (!glfwWindowShouldClose(window)) {
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    processInput();

    glm::mat4 view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);

    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    glm::mat4 projection = glm::perspective(glm::radians(45.f),
                                            (float)width / height, 0.1f, 100.f);

    shader->use();
    shader->setMat4("view", view);
    shader->setMat4("projection", projection);

    model->Draw(*shader);

    glfwSwapBuffers(window);
    glfwPollEvents();
  }
}

void Engine::shutdown() {
  if (window) {
    glfwDestroyWindow(window);
    window = nullptr;
  }
  glfwTerminate();
  running = false;
}
