#include "shader.h"
#include "../utils/shader_utils.h"
#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

Shader::Shader(const std::string &vertPath, const std::string &fragPath) {
  std::string vertexSourceStr = loadShaderFile(vertPath);
  std::string fragmentSourceStr = loadShaderFile(fragPath);

  if (vertexSourceStr.empty() || fragmentSourceStr.empty()) {
    std::cerr << "Failed to load shader files." << std::endl;
  }

  const char *vertexSource = vertexSourceStr.c_str();
  const char *fragmentSource = fragmentSourceStr.c_str();

  unsigned int vertShader = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertShader, 1, &vertexSource, NULL);
  glCompileShader(vertShader);

  unsigned int fragShader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragShader, 1, &fragmentSource, NULL);
  glCompileShader(fragShader);

  checkShaderCompile(vertShader, "Vertex");
  checkShaderCompile(fragShader, "Fragment");

  program = glCreateProgram();
  glAttachShader(program, vertShader);
  glAttachShader(program, fragShader);
  glLinkProgram(program);
  checkProgramLink(program);

  glDeleteShader(vertShader);
  glDeleteShader(fragShader);
}

Shader::~Shader() { glDeleteProgram(program); }

void Shader::use() const { glUseProgram(program); }

void Shader::setMat4(const std::string &name, const glm::mat4 &mat) const {
  glUniformMatrix4fv(glGetUniformLocation(program, name.c_str()), 1, GL_FALSE,
                     glm::value_ptr(mat));
}

void Shader::setVec3(const std::string &name, const glm::vec3 &vec) const {
  glUniform3fv(glGetUniformLocation(program, name.c_str()), 1,
               glm::value_ptr(vec));
}

// NOTE: This adds a sampler
// A sampler is just an integer that tells GLSL which texture unit (GL_TEXTURE0
// + i) to read
void Shader::setInt(const std::string &name, int value) const {
  glUniform1i(glGetUniformLocation(program, name.c_str()), value);
}
