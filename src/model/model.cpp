#include "model.h"
#include "glm/ext/matrix_transform.hpp"
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <glm/glm.hpp>
#include <stdexcept>

extern unsigned int TextureFromFile(const char *path,
                                    const std::string &directory);

Model::Model(const std::string &path) { loadModel(path); }

void Model::Draw(Shader &shader) const {
  shader.use();

  glm::mat4 modelMat(1.0f);
  modelMat = glm::translate(modelMat, transform.position);
  modelMat =
      glm::rotate(modelMat, glm::radians(transform.rotation.x), {1, 0, 0});
  modelMat =
      glm::rotate(modelMat, glm::radians(transform.rotation.y), {0, 1, 0});
  modelMat =
      glm::rotate(modelMat, glm::radians(transform.rotation.z), {0, 0, 1});
  modelMat = glm::scale(modelMat, transform.scale);

  shader.setMat4("model", modelMat);

  for (const auto &mesh : meshes)
    mesh->Draw(shader);
}

void Model::loadModel(const std::string &path) {
  Assimp::Importer importer;
  const aiScene *scene = importer.ReadFile(
      path, aiProcess_Triangulate | aiProcess_GenNormals | aiProcess_FlipUVs);

  if (!scene || !scene->mRootNode) {
    throw std::runtime_error(importer.GetErrorString());
  }

  directory = path.substr(0, path.find_last_of('/'));
  processNode(scene->mRootNode, scene);
}

void Model::processNode(aiNode *node, const aiScene *scene) {
  for (unsigned int i = 0; i < node->mNumMeshes; i++) {
    aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
    meshes.push_back(processMesh(mesh, scene));
  }

  for (unsigned int i = 0; i < node->mNumChildren; i++)
    processNode(node->mChildren[i], scene);
}

std::unique_ptr<Mesh> Model::processMesh(aiMesh *mesh, const aiScene *scene) {
  std::vector<Vertex> vertices;
  std::vector<unsigned int> indices;
  std::vector<Texture> textures;

  for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
    Vertex vertex;
    vertex.Position = {mesh->mVertices[i].x, mesh->mVertices[i].y,
                       mesh->mVertices[i].z};

    vertex.Normal = {mesh->mNormals[i].x, mesh->mNormals[i].y,
                     mesh->mNormals[i].z};

    if (mesh->mTextureCoords[0]) {
      vertex.TexCoords = {mesh->mTextureCoords[0][i].x,
                          mesh->mTextureCoords[0][i].y};
    } else {
      vertex.TexCoords = {0.0f, 0.0f};
    }

    vertices.push_back(vertex);
  }

  for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
    for (unsigned int j = 0; j < mesh->mFaces[i].mNumIndices; j++)
      indices.push_back(mesh->mFaces[i].mIndices[j]);
  }

  if (mesh->mMaterialIndex >= 0) {
    aiMaterial *mat = scene->mMaterials[mesh->mMaterialIndex];

    auto diffuseMaps =
        loadMaterialTextures(mat, aiTextureType_DIFFUSE, "texture_diffuse");
    auto specularMaps =
        loadMaterialTextures(mat, aiTextureType_SPECULAR, "texture_specular");

    textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
    textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
  }

  return std::make_unique<Mesh>(vertices, indices, textures);
}

std::vector<Texture> Model::loadMaterialTextures(aiMaterial *mat,
                                                 aiTextureType type,
                                                 const std::string &typeName) {
  std::vector<Texture> textures;

  for (unsigned int i = 0; i < mat->GetTextureCount(type); i++) {
    aiString str;
    mat->GetTexture(type, i, &str);

    bool skip = false;
    for (const auto &loadedTex : textures_loaded) {
      if (std::strcmp(loadedTex.path.c_str(), str.C_Str()) == 0) {
        textures.push_back(loadedTex);
        skip = true;
        break;
      }
    }

    if (!skip) {
      Texture texture;
      texture.id = TextureFromFile(str.C_Str(), directory);
      texture.type = typeName;
      texture.path = str.C_Str();
      textures.push_back(texture);
      textures_loaded.push_back(texture); // store for future reuse
    }
  }

  return textures;
}
