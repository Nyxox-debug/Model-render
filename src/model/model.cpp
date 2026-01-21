#include "model.h"
#include <glm/gtc/matrix_transform.hpp>
#include <memory>
#include <vector>

Model::Model(std::vector<std::unique_ptr<Mesh>> meshes)
    : meshes(std::move(meshes)) {}

void Model::Draw(Shader &shader) const {
  shader.use();

  // NOTE: Computation of model matrix from this model's Transform
  glm::mat4 modelMat = glm::mat4(1.0f);
  modelMat = glm::translate(modelMat, transform.position);
  modelMat = glm::rotate(modelMat, glm::radians(transform.rotation.x),
                         glm::vec3(1, 0, 0));
  modelMat = glm::rotate(modelMat, glm::radians(transform.rotation.y),
                         glm::vec3(0, 1, 0));
  modelMat = glm::rotate(modelMat, glm::radians(transform.rotation.z),
                         glm::vec3(0, 0, 1));
  modelMat = glm::scale(modelMat, transform.scale);

  shader.setMat4("model", modelMat);

  for (const auto &mesh : meshes)
    // NOTE: Because mesh is a pointer use mesh->Draw and not mesh.Draw()
    // mesh.Draw();
    mesh->Draw();
}
