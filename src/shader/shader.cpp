#include "shader.h"
#include "../utils/shader_utils.h"
#include <string>

Shader::Shader(const std::string &vertPath, const std::string &fragPath) {

  std::string vertexSourceStr = loadShaderFile(vertPath);
  if (vertexSourceStr.empty()) {
    std::cerr << "Failed to load vertex shader." << std::endl;
  }

  std::string fragmentSourceStr = loadShaderFile(fragPath);
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

  this->program = glCreateProgram();
  glAttachShader(program, vertShader);
  glAttachShader(program, fragShader);
  glLinkProgram(program);
  checkProgramLink(program);

  glDeleteShader(vertShader);
  glDeleteShader(fragShader);
}

void Shader::use() const { glUseProgram(this->program); }

Shader::~Shader() { glDeleteProgram(this->program); }
