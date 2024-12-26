#include "mesh.h"
#include <glad/glad.h>

static unsigned int bindVertexArrayObject()
{
    // Vertex array object.
    unsigned int vertex_array_object;
    glGenVertexArrays(1, &vertex_array_object);
    glBindVertexArray(vertex_array_object);
    return vertex_array_object;
}

static unsigned int bindVertexBufferObject(vector<Vertex> vertices)
{
    // Vertex buffer object.
    unsigned int vertex_buffer_object;
    glGenBuffers(1, &vertex_buffer_object);
    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_object);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);
    return vertex_buffer_object;
}

static unsigned int bindElementBufferObject(vector<unsigned int> indices)
{
    // Element Buffer object.
    unsigned int element_buffer_object;
    glGenBuffers(1, &element_buffer_object);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, element_buffer_object);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);
    return element_buffer_object;
}

Mesh::Mesh(const vector<Vertex>& vertices, const vector<unsigned int>& indices, const vector<Texture>& textures)
{
    this->vertices = vertices;
    this->indices = indices;
    this->textures = textures;

    setupMesh();
}

void Mesh::draw(Shader shader) {
    unsigned int diffuseIdx = 0;
    unsigned int specularIdx = 0;
    for (unsigned int i = 0; i < textures.size(); i++) {
        glActiveTexture(GL_TEXTURE0 + i);
        if (textures[i].type == "texture_diffuse") {
            diffuseIdx++;
        } else if (textures[i].type == "texture_specular") {
            specularIdx++;
        }
        shader.setInt(("material." + textures[i].type + '[' + std::to_string(diffuseIdx) + ']'), i);
        glBindTexture(GL_TEXTURE_2D, textures[i].id);
    }

    glBindVertexArray(vao);     // Bind VAO.
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);       // Remove VAO.
}

void Mesh::setupMesh()
{
    // Initialize objects.
    this->vao = bindVertexArrayObject();
    this->vbo = bindVertexBufferObject(this->vertices);
    this->ebo = bindElementBufferObject(this->indices);

    // Set vertex positions.
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
    // Set vertex normals.
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
    // Set vertex normals.
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, tex_coords));

    glBindVertexArray(0);
}