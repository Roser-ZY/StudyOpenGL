#include "shader.h"

Shader::Shader(const char* vertex_path, const char* fragment_path)
{
    std::string vertex_code;
    std::string fragment_code;
    std::ifstream vertex_shader_file;
    std::ifstream fragment_shader_file;

    vertex_shader_file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    fragment_shader_file.exceptions(std::ifstream::failbit | std::ifstream::badbit);

    try {
        vertex_shader_file.open(vertex_path);
        fragment_shader_file.open(fragment_path);

        // Reading.
        std::stringstream vertexShaderStream;
        std::stringstream fragmentShaderStream;
        vertexShaderStream << vertex_shader_file.rdbuf();
        fragmentShaderStream << fragment_shader_file.rdbuf();

        vertex_shader_file.close();
        fragment_shader_file.close();

        // Get the string of file.
        vertex_code = vertexShaderStream.str();
        fragment_code = fragmentShaderStream.str();
    } catch (std::ifstream::failure e) {
        std::cout << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ" << std::endl;
    }

    const char* vertex_shader_code = vertex_code.c_str();
    const char* fragment_shader_code = fragment_code.c_str();

    int success;
    char info_log[512];

    // Compile vertex shader.
    unsigned int vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex_shader, 1, &vertex_shader_code, NULL);
    glCompileShader(vertex_shader);
    glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertex_shader, 512, NULL, info_log);
        std::cout << "ERROR:SHADER::VERTEX::COMPILATION_FAILED\n" << info_log << std::endl;
    }
    
    // Compile fragment shader.
    unsigned int fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment_shader, 1, &fragment_shader_code, NULL);
    glCompileShader(fragment_shader);
    glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fragment_shader, 512, NULL, info_log);
        std::cout << "ERROR:SHADER::FRAGMENT::COMPILATION_FAILED\n" << info_log << std::endl;
    }

    // Link the shader program.
    shader_program = glCreateProgram();
    glAttachShader(shader_program, vertex_shader);
    glAttachShader(shader_program, fragment_shader);
    glLinkProgram(shader_program);

    glGetProgramiv(shader_program, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shader_program, 512, NULL, info_log);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << info_log << std::endl;
    }
    // Delete shaders.
    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);
}

Shader::~Shader() {
    glDeleteProgram(shader_program);
}

void Shader::use()
{
    glUseProgram(shader_program);
}

void Shader::setBool(const std::string& name, bool value) const
{
    glUniform1i(glGetUniformLocation(shader_program, name.c_str()), (int)value);
}

void Shader::setInt(const std::string& name, int value) const
{
    glUniform1i(glGetUniformLocation(shader_program, name.c_str()), value);
}

void Shader::setFloat(const std::string& name, float value) const
{
    glUniform1f(glGetUniformLocation(shader_program, name.c_str()), value);
}


void Shader::setMat4(const std::string& name, const glm::mat4& value) const
{
    glUniformMatrix4fv(glGetUniformLocation(shader_program, name.c_str()), 1, GL_FALSE, glm::value_ptr(value));
}

void Shader::setVec3(const std::string& name, int x, int y, int z) const
{
    glUniform3f(glGetUniformLocation(shader_program, name.c_str()), x, y, z);
}

void Shader::setVec3(const std::string& name, const glm::vec3& value) const
{
    glUniform3fv(glGetUniformLocation(shader_program, name.c_str()), 1, &value[0]);
}