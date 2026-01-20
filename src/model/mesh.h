#pragma once
#include <glad/glad.h>
#include <vector>

class Mesh {
private:
  unsigned int VAO, VBO, EBO;
  unsigned int indexCount; // NOTE: Needed for glDrawElements

public:
  Mesh(const std::vector<float> &vertices,
       const std::vector<unsigned int> &indices);
  void Draw();
};
