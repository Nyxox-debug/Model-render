#pragma once

#include <memory>

class Mesh;

struct GLFWwindow;

class Engine {
private:
  GLFWwindow *window = nullptr;
  bool running = false;
  unsigned int shaderProgram;
  std::unique_ptr<Mesh> mesh;

public:
  Engine();
  ~Engine();

  bool init();
  void run();
  void shutdown();
};
