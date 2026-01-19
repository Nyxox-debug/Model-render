#pragma once

struct GLFWwindow;

class Engine {
private:
  GLFWwindow *window = nullptr;
  bool running = false;
  unsigned int shaderProgram;
  unsigned int VAO;
  unsigned int VBO;
  unsigned int EBO;

public:
  bool init();
  void run();
  void shutdown();
};
