#pragma once

struct GLFWwindow;

class Engine {
private:
  GLFWwindow *window = nullptr;
  bool running = false;

public:
  bool init();
  void run();
  void shutdown();
};
