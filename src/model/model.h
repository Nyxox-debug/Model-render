#pragma once
#include "../shader/shader.h"
#include "mesh.h"
#include "transform.h"
#include <assimp/scene.h>
#include <memory>
#include <string>
#include <vector>

class Model {
public:
  Model(const std::string &path);
  void Draw(Shader &shader) const;

  Transform transform;

private:
  std::vector<std::unique_ptr<Mesh>> meshes;
  std::vector<Texture> textures_loaded;
  std::string directory;

  void loadModel(const std::string &path);
  void processNode(aiNode *node, const aiScene *scene);
  std::unique_ptr<Mesh> processMesh(aiMesh *mesh, const aiScene *scene);
  std::vector<Texture> loadMaterialTextures(aiMaterial *mat, aiTextureType type,
                                            const std::string &typeName);
};

// #include "../shader/shader.h"
// #include "mesh.h"
// #include "transform.h"
// #include <memory>
//
// class Model {
// public:
//   Model(std::vector<std::unique_ptr<Mesh>> meshes);
//   void Draw(Shader &shader) const;
//
//   Transform transform;
//
// private:
//   std::vector<std::unique_ptr<Mesh>> meshes;
// };
