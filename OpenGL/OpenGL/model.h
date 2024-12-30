#pragma once
#ifndef MODEL_H
#define MODEL_H

#include <vector>
#include <string>

#include "assimp/scene.h"

#include "shader.h"
#include "mesh.h"

class Model {
public:
    Model(char* path);
    void draw(Shader shader);
private:
    void loadModel(std::string path);
    void processNode(aiNode* node, const aiScene* scene);
    Mesh processMesh(aiMesh* mesh, const aiScene* scene);
    std::vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);

    std::vector<Mesh> meshes_;
    std::string directory_;
};

#endif