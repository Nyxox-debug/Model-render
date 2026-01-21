#pragma once

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

public:
  Engine();
  ~Engine();

  bool init();
  void run();
  void shutdown();
};
