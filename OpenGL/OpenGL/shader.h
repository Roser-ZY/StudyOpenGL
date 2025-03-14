#pragma once
#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>
#include <gtc/type_ptr.hpp>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

class Shader {
public:
    unsigned int shader_program;

    Shader(const char* vertexPath, const char* fragmentPath);
    ~Shader();

    void use();
    // uniform ���ߺ���
    void setBool(const std::string& name, bool value) const;
    void setInt(const std::string& name, int value) const;
    void setFloat(const std::string& name, float value) const;
    void setMat4(const std::string& name, const glm::mat4& value) const;
    void setVec3(const std::string& name, float x, float y, float z) const;
    void setVec3(const std::string& name, const glm::vec3& value) const;
    ;};

#endif