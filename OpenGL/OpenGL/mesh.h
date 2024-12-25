#pragma once
#ifndef MESH_H
#define MESH_H
#include <glm.hpp>
#include <string>
#include <vector>

#include "shader.h"

using std::string;
using std::vector;

struct Vertex {
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec3 tex_coords;
};

struct Texture {
    unsigned int id;
    string type;
};

class Mesh {
public:
    vector<Vertex> vertices;
    vector<unsigned int> indices;
    vector<Texture> textures;

    Mesh(const vector<Vertex>& vertices, const vector<unsigned int>& indices, const vector<Texture>& textures);
    void draw(Shader shader);

private:
    void setupMesh();

    unsigned int vao;
    unsigned int vbo;
    unsigned int ebo;
};

#endif