#include "../shader/shader.h"
#include "mesh.h"
#include <memory>

class Model {
public:
  Model(std::vector<std::unique_ptr<Mesh>> meshes);
  void Draw(Shader &shader) const;

private:
  std::vector<std::unique_ptr<Mesh>> meshes;
};
