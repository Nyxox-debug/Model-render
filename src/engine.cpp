#include "engine.h"
#include "glm/ext/matrix_clip_space.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "model/mesh.h"
#include "model/model.h"
#include "shader/shader.h"
#include <memory>

// NOTE: To future self, Always include glad first before glfw, but in this case
// it works because glad is included in utils/shader_utils.h
#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <iostream>

Engine::Engine() = default;
Engine::~Engine() = default;

// float vertices[] = {
//     0.5f,  0.5f,  0.0f, // top right
//     0.5f,  -0.5f, 0.0f, // bottom right
//     -0.5f, -0.5f, 0.0f, // bottom left
//     -0.5f, 0.5f,  0.0f  // top left
// };
// unsigned int indices[] = {
//     // note that we start from 0!
//     0, 1, 3, // first triangle
//     1, 2, 3  // second triangle
// };

std::vector<float> vertices = {0.5f,  0.5f,  0.0f, 0.5f,  -0.5f, 0.0f,
                               -0.5f, -0.5f, 0.0f, -0.5f, 0.5f,  0.0f};

std::vector<unsigned int> indices = {0, 1, 3, 1, 2, 3};

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

  glViewport(0, 0, 800, 600);

  // Mesh
  // this->mesh = std::make_unique<Mesh>(vertices, indices);
  // std::vector<Mesh> model = {*mesh};

  std::vector<std::unique_ptr<Mesh>> meshes;
  // NOTE: To self, you used make_unique because it create's an object owned by
  meshes.push_back(std::make_unique<Mesh>(vertices, indices));
  this->model = std::make_unique<Model>(std::move(meshes));

  // Transform
  this->model->transform.position =
      glm::vec3(0.0f, 0.0f, -0.5f);                  // just slightly back
  this->model->transform.scale = glm::vec3(1.0f);    // default size
  this->model->transform.rotation = glm::vec3(0.0f); // no rotation

  // Shaders
  this->shader = std::make_unique<Shader>("../res/shaders/def.vert",
                                          "../res/shaders/def.frag");

  running = true;
  return true;
}

void Engine::run() {
  if (!running)
    return;

  while (!glfwWindowShouldClose(window)) {
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    // Camera Setup
    glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, -3));
    glm::mat4 projection =
        glm::perspective(glm::radians(45.f), 800.f / 600.f, 0.1f, 100.f);

    shader->use();
    shader->setMat4("view", view);
    shader->setMat4("projection", projection);

    model->Draw(*this->shader); // sets 'model' inside Model::Draw()

    // this->shader->use();
    this->model->Draw(*this->shader);
    // this->mesh->Draw();

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
