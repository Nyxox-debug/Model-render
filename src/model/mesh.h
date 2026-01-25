#pragma once
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <string>
#include <vector>

#define MAX_BONE_INFLUENCE 4

struct Vertex {
  glm::vec3 Position;
  glm::vec3 Normal;
  glm::vec2 TexCoords;
  glm::vec3 Tangent;
  glm::vec3 Bitangent;
  int m_BoneIDs[MAX_BONE_INFLUENCE];
  float m_Weights[MAX_BONE_INFLUENCE];
};

struct Texture {
  unsigned int id;
  std::string type;
  std::string path;
};

class Shader;

class Mesh {
public:
  std::vector<Vertex> vertices;
  std::vector<unsigned int> indices;
  std::vector<Texture> textures;

  Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices,
       std::vector<Texture> textures);

  void Draw(Shader &shader) const;

private:
  unsigned int VAO, VBO, EBO;
  void setupMesh();
};
// #pragma once
// #include "../shader/shader.h"
// #include "texture.h"
// #include "vertex.h"
// #include <glad/glad.h>
// #include <vector>
//
// class Mesh {
// private:
//   unsigned int VAO, VBO, EBO;
//   unsigned int indexCount; // NOTE: Needed for glDrawElements
//   void setupMesh();
//
// public:
//   std::vector<Vertex> vertices;
//   std::vector<unsigned int> indices;
//   std::vector<Texture> textures;
//   Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indicies,
//        std::vector<Texture> textures = {});
//   void Draw(Shader &shader) const;
// };
