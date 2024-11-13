#include <iostream>
#include "glad/glad.h"
#include <GLFW/glfw3.h>

#include "shader.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>

void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

unsigned int bindVertexArrayObject()
{
    // Vertex array object.
    unsigned int vertex_array_object;
    glGenVertexArrays(1, &vertex_array_object);
    glBindVertexArray(vertex_array_object);
    return vertex_array_object;
}

unsigned int bindVertexBufferObject(float vertices[], unsigned int vertices_size)
{
    // Vertex buffer object.
    unsigned int vertex_buffer_object;
    glGenBuffers(1, &vertex_buffer_object);
    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_object);
    glBufferData(GL_ARRAY_BUFFER, vertices_size, vertices, GL_STATIC_DRAW);
    return vertex_buffer_object;
}

unsigned int bindElementBufferObject(unsigned int indices[], unsigned int indices_size)
{
    // Element Buffer object.
    unsigned int element_buffer_object;
    glGenBuffers(1, &element_buffer_object);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, element_buffer_object);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices_size, indices, GL_STATIC_DRAW);
    return element_buffer_object;
}

unsigned int generateTexture(const char* image_path, int active_texture, int rgb_mode)
{
    unsigned int texture;
    glGenTextures(1, &texture);
    glActiveTexture(active_texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Flip vertically.
    stbi_set_flip_vertically_on_load(true);

    // Load texture.
    int width = 0;
    int height = 0;
    int nrChannels = 0;
    unsigned char* data = stbi_load(image_path, &width, &height, &nrChannels, 0);
    if (data != nullptr) {
        glTexImage2D(GL_TEXTURE_2D, 0, rgb_mode, width, height, 0, rgb_mode, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    } else {
        std::cout << "Failed to load texture." << std::endl;
    }
    // Recycle the image data.
    stbi_image_free(data);
    return texture;
}

void drawTriangle(GLFWwindow* window, float vertices[], unsigned int vertice_size)
{
    // Vertex array object.
    unsigned int vertex_array_object = bindVertexArrayObject();
    // Vertex buffer object.
    unsigned int vertex_buffer_object = bindVertexBufferObject(vertices, vertice_size);
    // Attributes.
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Compile.
    Shader shader("D:/Turotials/StudyOpenGL/OpenGL/OpenGL/shader.vs",
                  "D:/Turotials/StudyOpenGL/OpenGL/OpenGL/shader.fs");

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window)) {
        /* Render here */
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        processInput(window);

        // Draw the triangle.
        shader.use();
        // Because this code only draw one triangle, so we don't need to bind vertex array object every time.
        //glBindVertexArray(vertex_array_object);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    // Optianl, de-allocate all resource once ther've outlived their purpose.
    glDeleteVertexArrays(1, &vertex_array_object);
    glDeleteBuffers(1, &vertex_buffer_object);
}

void drawRectangle(GLFWwindow* window)
{
    float vertices[] = {
        // ---- 位置 ----      ---- 颜色 ----    -- 纹理 --
        0.5f,  0.5f,  0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,  // 右上
        0.5f,  -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,  // 右下
        -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,  // 左下
        -0.5f, 0.5f,  0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f   // 左上
    };
    unsigned int indices[] = {0, 1, 3, 1, 2, 3};


    // Vertex array object.
    unsigned int vertex_array_object = bindVertexArrayObject();

    // Vertex buffer object.
    unsigned int vertex_buffer_object = bindVertexBufferObject(vertices, sizeof(vertices));

    // Element buffer object.
    unsigned int element_buffer_object = bindElementBufferObject(indices, sizeof(indices));

    // Texture.
    unsigned int texture1 = generateTexture("D:\\Turotials\\StudyOpenGL\\OpenGL\\Assets\\container.jpg", GL_TEXTURE0, GL_RGB);
    unsigned int texture2 = generateTexture("D:\\Turotials\\StudyOpenGL\\OpenGL\\Assets\\awesomeface.png", GL_TEXTURE1, GL_RGBA);

    // Attributes.
    // Vertex positions.
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // Vertex colors.
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    // Texture.
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    // Compile.
    Shader shader("D:/Turotials/StudyOpenGL/OpenGL/OpenGL/shader.vs",
                  "D:/Turotials/StudyOpenGL/OpenGL/OpenGL/shader.fs");

    shader.use();
    shader.setInt("texture1", 0);
    shader.setInt("texture2", 1);

    //// Wireframe mode.
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window)) {
        /* Render here */
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        processInput(window);

        // Draw the triangle.
        shader.use();

        // Transform.
        glm::mat4 trans = glm::mat4(1.0f);
        trans = glm::translate(trans, glm::vec3(0.5f, -0.5f, 0.0f));
        trans = glm::rotate(trans, (float)glfwGetTime(), glm::vec3(0.0f, 0.0f, 1.0f));
        // Send to the shader.
        unsigned int transform_loc = glGetUniformLocation(shader.shader_program, "transform");
        glUniformMatrix4fv(transform_loc, 1, GL_FALSE, glm::value_ptr(trans));

        // Because this code only draw one triangle, so we don't need to bind vertex array object every time.
        //glBindVertexArray(vertex_array_object);
        //glDrawArrays(GL_TRIANGLES, 0, 3);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    // Optianl, de-allocate all resource once ther've outlived their purpose.
    glDeleteVertexArrays(1, &vertex_array_object);
    glDeleteBuffers(1, &vertex_buffer_object);
    glDeleteBuffers(1, &element_buffer_object);
}

int main(void)
{
    /* Initialize the library */
    if (!glfwInit())
        return -1;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    /* Create a windowed mode window and its OpenGL context */
    GLFWwindow* window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
    if (!window) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    //float vertices[] = {-0.5f, -0.5f, 0.0f, 0.5f, -0.5f, 0.0f, 0.0f, 0.5f, 0.0f};
    //drawTriangle(window, vertices, sizeof(vertices));

    drawRectangle(window);

    glfwTerminate();
    return 0;
}
