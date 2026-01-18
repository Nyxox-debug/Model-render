#include "engine.h"
#include "utils/shader_utils.h"

// NOTE: Always include glad first before glfw, but in this case it works
// because glad is included in utils/shader_utils.h
#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <iostream>

float vertices[] = {
    -0.5f, -0.25f, 0.0f, // bottom-left
    0.5f,  -0.25f, 0.0f, // bottom-right
    0.0f,  0.5f,   0.0f  // top-center
};

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

  // TODO: Extract loading Vertex data to a class for main engine
  unsigned int VBO;
  glGenVertexArrays(1, &this->VAO);
  glGenBuffers(1, &VBO);
  glBindVertexArray(VAO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  // Linking Vertex Attribs

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(0);

  // Shaders
  // TODO: Also Extract the Shader Code
  std::string vertexSourceStr = loadShaderFile("../res/shaders/def.vert");
  if (vertexSourceStr.empty()) {
    std::cerr << "Failed to load vertex shader." << std::endl;
  }

  std::string fragmentSourceStr = loadShaderFile("../res/shaders/def.frag");
  if (fragmentSourceStr.empty()) {
    std::cerr << "Failed to load fragment shader." << std::endl;
  }

  const char *vertexSource = vertexSourceStr.c_str();
  const char *fragmentSource = fragmentSourceStr.c_str();

  unsigned int vertShader = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertShader, 1, &vertexSource, NULL);
  glCompileShader(vertShader);

  unsigned int fragShader;
  fragShader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragShader, 1, &fragmentSource, NULL);
  glCompileShader(fragShader);

  // Optional Check for Vertex and Fragment Shader
  checkShaderCompile(vertShader, "Vertex");
  checkShaderCompile(fragShader, "Fragment");

  this->shaderProgram = glCreateProgram();
  glAttachShader(shaderProgram, vertShader);
  glAttachShader(shaderProgram, fragShader);
  glLinkProgram(shaderProgram);
  checkProgramLink(shaderProgram);

  glDeleteShader(vertShader);
  glDeleteShader(fragShader);

  running = true;
  return true;
}

void Engine::run() {
  if (!running)
    return;

  while (!glfwWindowShouldClose(window)) {
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(shaderProgram);
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 3);

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
