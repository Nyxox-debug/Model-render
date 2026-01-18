#pragma once

struct GLFWwindow;

class Engine {
private:
  GLFWwindow *window = nullptr;
  bool running = false;
  unsigned int shaderProgram;
  unsigned int VAO;

public:
  bool init();
  void run();
  void shutdown();
};
