#pragma once
#include <string>
#include <glm/glm.hpp>

class Shader {
public:
    Shader(const std::string &vertPath, const std::string &fragPath);
    ~Shader();

    void use() const;

    void setMat4(const std::string &name, const glm::mat4 &mat) const;
    void setVec3(const std::string &name, const glm::vec3 &vec) const;

private:
    unsigned int program;
};
