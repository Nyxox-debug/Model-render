#include "mesh.h"
#include "../shader/shader.h"
#include <cstddef>

Mesh::Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices,
           std::vector<Texture> textures)
    : vertices(std::move(vertices)), indices(std::move(indices)),
      textures(std::move(textures)) {
  setupMesh();
}

void Mesh::setupMesh() {
  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);
  glGenBuffers(1, &EBO);

  glBindVertexArray(VAO);

  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex),
               vertices.data(), GL_STATIC_DRAW);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int),
               indices.data(), GL_STATIC_DRAW);

  // positions
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)0);

  // normals
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                        (void *)offsetof(Vertex, Normal));

  // texcoords
  glEnableVertexAttribArray(2);
  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                        (void *)offsetof(Vertex, TexCoords));

  glBindVertexArray(0);
}

void Mesh::Draw(Shader &shader) const {
  unsigned int diffuseNr = 1;
  unsigned int specularNr = 1;

  for (unsigned int i = 0; i < textures.size(); i++) {
    glActiveTexture(GL_TEXTURE0 + i);

    std::string number;
    std::string name = textures[i].type;

    if (name == "texture_diffuse")
      number = std::to_string(diffuseNr++);
    else if (name == "texture_specular")
      number = std::to_string(specularNr++);

    shader.setInt("material." + name + number, i);
    glBindTexture(GL_TEXTURE_2D, textures[i].id);
  }

  glBindVertexArray(VAO);
  glDrawElements(GL_TRIANGLES, static_cast<unsigned int>(indices.size()),
                 GL_UNSIGNED_INT, 0);
  glBindVertexArray(0);
  glActiveTexture(GL_TEXTURE0);
}

// #include "shader.h"
// #include "../utils/shader_utils.h"
// #include <glad/glad.h>
// #include <glm/gtc/type_ptr.hpp>
// #include <iostream>
//
// Shader::Shader(const std::string &vertPath, const std::string &fragPath) {
//     std::string vertexSourceStr = loadShaderFile(vertPath);
//     std::string fragmentSourceStr = loadShaderFile(fragPath);
//
//     if (vertexSourceStr.empty() || fragmentSourceStr.empty()) {
//         std::cerr << "Failed to load shader files." << std::endl;
//     }
//
//     const char* vertexSource = vertexSourceStr.c_str();
//     const char* fragmentSource = fragmentSourceStr.c_str();
//
//     unsigned int vertShader = glCreateShader(GL_VERTEX_SHADER);
//     glShaderSource(vertShader, 1, &vertexSource, NULL);
//     glCompileShader(vertShader);
//
//     unsigned int fragShader = glCreateShader(GL_FRAGMENT_SHADER);
//     glShaderSource(fragShader, 1, &fragmentSource, NULL);
//     glCompileShader(fragShader);
//
//     checkShaderCompile(vertShader, "Vertex");
//     checkShaderCompile(fragShader, "Fragment");
//
//     program = glCreateProgram();
//     glAttachShader(program, vertShader);
//     glAttachShader(program, fragShader);
//     glLinkProgram(program);
//     checkProgramLink(program);
//
//     glDeleteShader(vertShader);
//     glDeleteShader(fragShader);
// }
//
// Shader::~Shader() {
//     glDeleteProgram(program);
// }
//
// void Shader::use() const {
//     glUseProgram(program);
// }
//
// void Shader::setMat4(const std::string &name, const glm::mat4 &mat) const {
//     glUniformMatrix4fv(glGetUniformLocation(program, name.c_str()), 1,
//     GL_FALSE, glm::value_ptr(mat));
// }
//
// void Shader::setVec3(const std::string &name, const glm::vec3 &vec) const {
//     glUniform3fv(glGetUniformLocation(program, name.c_str()), 1,
//     glm::value_ptr(vec));
// }
