#include "model.h"
#include <memory>
#include <vector>

Model::Model(std::vector<std::unique_ptr<Mesh>> meshes)
    : meshes(std::move(meshes)) {}

void Model::Draw(Shader &shader) const {
  shader.use();
  for (const auto &mesh : meshes)
    // NOTE: Because mesh is a pointer use mesh->Draw and not mesh.Draw()
    // mesh.Draw();
    mesh->Draw();
}
