#include "camera.h"
#include "glad/glad.h"
#include "shader.h"
#include "model.h"

#include <GLFW/glfw3.h>
#include <iostream>
#include <vector>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>

static Camera camera;
static float delta_time = 0.0f;  // 当前帧与上一帧的时间差
static float last_frame = 0.0f;  // 上一帧的时间

void processKeyboard(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        camera.processKeyboard(CameraMovement::FORWARD, delta_time);
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        camera.processKeyboard(CameraMovement::BACKWARD, delta_time);
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        camera.processKeyboard(CameraMovement::LEFT, delta_time);
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        camera.processKeyboard(CameraMovement::RIGHT, delta_time);
    }
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) {
        camera.processKeyboard(CameraMovement::UP, delta_time);
    }
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {
        camera.processKeyboard(CameraMovement::DOWN, delta_time);
    }
}

void processMouseMovement(GLFWwindow* window, double xpos, double ypos)
{
    static bool first_mouse_movement = true;
    static float last_x = 400;
    static float last_y = 300;
    if (first_mouse_movement) {
        last_x = xpos;
        last_y = ypos;
        first_mouse_movement = false;
    }
    float xoffset = xpos - last_x;
    float yoffset = last_y - ypos;  // The window coordinate is reversed with graphics.
    last_x = xpos;
    last_y = ypos;
    camera.processMouseMovement(xoffset, yoffset, true);
}

void processMouseScroll(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.processMouseScroll(yoffset);
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

// Getting started.
namespace GettingStarted {
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

            processKeyboard(window);

            // Draw the triangle.
            shader.use();
            // Because this code only draw one triangle, so we don't need to bind vertex array object every time.
            // glBindVertexArray(vertex_array_object);
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
        unsigned int texture1 =
            generateTexture("D:\\Turotials\\StudyOpenGL\\OpenGL\\Assets\\container.jpg", GL_TEXTURE0, GL_RGB);
        unsigned int texture2 =
            generateTexture("D:\\Turotials\\StudyOpenGL\\OpenGL\\Assets\\awesomeface.png", GL_TEXTURE1, GL_RGBA);

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

        // Coordinate.
        glm::mat4 model(1.0f);
        model = glm::rotate(model, glm::radians(-55.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        glm::mat4 view(1.0f);
        view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
        glm::mat4 projection(1.0f);
        projection = glm::perspective(glm::radians(45.0f), (float)(640.0 / 480.0), 0.1f, 100.0f);

        int modelLoc = glGetUniformLocation(shader.shader_program, "model");
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        int viewLoc = glGetUniformLocation(shader.shader_program, "view");
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
        int projectionLoc = glGetUniformLocation(shader.shader_program, "projection");
        glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

        //// Wireframe mode.
        // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

        /* Loop until the user closes the window */
        while (!glfwWindowShouldClose(window)) {
            /* Render here */
            glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);

            processKeyboard(window);

            // Draw the triangle.
            shader.use();

            // Because this code only draw one triangle, so we don't need to bind vertex array object every time.
            // glBindVertexArray(vertex_array_object);
            // glDrawArrays(GL_TRIANGLES, 0, 3);
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

    void drawBox(GLFWwindow* window)
    {
        float vertices[] = {
            //      位置              纹理
            -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 0.5f,  -0.5f, -0.5f, 1.0f, 0.0f, 0.5f,  0.5f,  -0.5f, 1.0f, 1.0f,
            0.5f,  0.5f,  -0.5f, 1.0f, 1.0f, -0.5f, 0.5f,  -0.5f, 0.0f, 1.0f, -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,

            -0.5f, -0.5f, 0.5f,  0.0f, 0.0f, 0.5f,  -0.5f, 0.5f,  1.0f, 0.0f, 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
            0.5f,  0.5f,  0.5f,  1.0f, 1.0f, -0.5f, 0.5f,  0.5f,  0.0f, 1.0f, -0.5f, -0.5f, 0.5f,  0.0f, 0.0f,

            -0.5f, 0.5f,  0.5f,  1.0f, 0.0f, -0.5f, 0.5f,  -0.5f, 1.0f, 1.0f, -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
            -0.5f, -0.5f, -0.5f, 0.0f, 1.0f, -0.5f, -0.5f, 0.5f,  0.0f, 0.0f, -0.5f, 0.5f,  0.5f,  1.0f, 0.0f,

            0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.5f,  0.5f,  -0.5f, 1.0f, 1.0f, 0.5f,  -0.5f, -0.5f, 0.0f, 1.0f,
            0.5f,  -0.5f, -0.5f, 0.0f, 1.0f, 0.5f,  -0.5f, 0.5f,  0.0f, 0.0f, 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

            -0.5f, -0.5f, -0.5f, 0.0f, 1.0f, 0.5f,  -0.5f, -0.5f, 1.0f, 1.0f, 0.5f,  -0.5f, 0.5f,  1.0f, 0.0f,
            0.5f,  -0.5f, 0.5f,  1.0f, 0.0f, -0.5f, -0.5f, 0.5f,  0.0f, 0.0f, -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,

            -0.5f, 0.5f,  -0.5f, 0.0f, 1.0f, 0.5f,  0.5f,  -0.5f, 1.0f, 1.0f, 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
            0.5f,  0.5f,  0.5f,  1.0f, 0.0f, -0.5f, 0.5f,  0.5f,  0.0f, 0.0f, -0.5f, 0.5f,  -0.5f, 0.0f, 1.0f};

        // Vertex array object.
        unsigned int vertex_array_object = bindVertexArrayObject();

        // Vertex buffer object.
        unsigned int vertex_buffer_object = bindVertexBufferObject(vertices, sizeof(vertices));

        // Texture.
        unsigned int texture1 =
            generateTexture("D:\\Turotials\\StudyOpenGL\\OpenGL\\Assets\\container.jpg", GL_TEXTURE0, GL_RGB);
        unsigned int texture2 =
            generateTexture("D:\\Turotials\\StudyOpenGL\\OpenGL\\Assets\\awesomeface.png", GL_TEXTURE1, GL_RGBA);

        // Attributes.
        // Vertex positions.
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        // Texture.
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);

        // Compile.
        Shader shader("D:/Turotials/StudyOpenGL/OpenGL/OpenGL/box_shader.vs",
                      "D:/Turotials/StudyOpenGL/OpenGL/OpenGL/box_shader.fs");

        shader.use();
        shader.setInt("texture1", 0);
        shader.setInt("texture2", 1);

        // Coordinate.
        glm::vec3 cube_positions[] = {glm::vec3(0.0f, 0.0f, 0.0f),    glm::vec3(2.0f, 5.0f, -15.0f),
                                      glm::vec3(-1.5f, -2.2f, -2.5f), glm::vec3(-3.8f, -2.0f, -12.3f),
                                      glm::vec3(2.4f, -0.4f, -3.5f),  glm::vec3(-1.7f, 3.0f, -7.5f),
                                      glm::vec3(1.3f, -2.0f, -2.5f),  glm::vec3(1.5f, 2.0f, -2.5f),
                                      glm::vec3(1.5f, 0.2f, -1.5f),   glm::vec3(-1.3f, 1.0f, -1.5f)};

        glm::mat4 projection(1.0f);
        projection = glm::perspective(glm::radians(60.0f), (float)(640.0 / 480.0), 0.1f, 10.0f);
        shader.setMat4("projection", projection);

        //// Wireframe mode.
        // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

        glEnable(GL_DEPTH_TEST);
        // Capture the mouse in the window.
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        // Set call back function to process mouse movement.
        glfwSetCursorPosCallback(window, processMouseMovement);
        // Set call back function to process mouse scroll.
        glfwSetScrollCallback(window, processMouseScroll);

        /* Loop until the user closes the window */
        while (!glfwWindowShouldClose(window)) {
            /* Render here */
            glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            float current_frame = glfwGetTime();
            delta_time = current_frame - last_frame;
            last_frame = current_frame;

            processKeyboard(window);

            // Draw the triangle.
            shader.use();

            // Rotate boxes.
            for (unsigned int i = 0; i < 10; ++i) {
                glm::mat4 model(1.0f);
                model = glm::translate(model, cube_positions[i]);
                float angle = 20.0f * i;
                model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
                model = glm::rotate(model, (float)glfwGetTime() * glm::radians(20.0f), glm::vec3(0.5f, 1.0f, 0.0f));
                shader.setMat4("model", model);
                glDrawArrays(GL_TRIANGLES, 0, 36);
            }

            // Camera.
            glm::mat4 view = camera.getViewMatrix();
            shader.setMat4("view", view);

            /* Swap front and back buffers */
            glfwSwapBuffers(window);

            /* Poll for and process events */
            glfwPollEvents();
        }

        // Optianl, de-allocate all resource once ther've outlived their purpose.
        glDeleteVertexArrays(1, &vertex_array_object);
        glDeleteBuffers(1, &vertex_buffer_object);
    }
}  // namespace GettingStarted

// Lighting
namespace Lighting {
    glm::vec3 light_pos(1.2f, 1.0f, 2.0f);
    void drawBox(GLFWwindow* window)
    {
        float vertices[] = {
            // positions          // normals           // texture coords
            -0.5f, -0.5f, -0.5f, 0.0f,  0.0f,  -1.0f, 0.0f, 0.0f, 0.5f,  -0.5f, -0.5f, 0.0f,  0.0f,  -1.0f, 1.0f, 0.0f,
            0.5f,  0.5f,  -0.5f, 0.0f,  0.0f,  -1.0f, 1.0f, 1.0f, 0.5f,  0.5f,  -0.5f, 0.0f,  0.0f,  -1.0f, 1.0f, 1.0f,
            -0.5f, 0.5f,  -0.5f, 0.0f,  0.0f,  -1.0f, 0.0f, 1.0f, -0.5f, -0.5f, -0.5f, 0.0f,  0.0f,  -1.0f, 0.0f, 0.0f,

            -0.5f, -0.5f, 0.5f,  0.0f,  0.0f,  1.0f,  0.0f, 0.0f, 0.5f,  -0.5f, 0.5f,  0.0f,  0.0f,  1.0f,  1.0f, 0.0f,
            0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f, 1.0f, 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f, 1.0f,
            -0.5f, 0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f, 1.0f, -0.5f, -0.5f, 0.5f,  0.0f,  0.0f,  1.0f,  0.0f, 0.0f,

            -0.5f, 0.5f,  0.5f,  -1.0f, 0.0f,  0.0f,  1.0f, 0.0f, -0.5f, 0.5f,  -0.5f, -1.0f, 0.0f,  0.0f,  1.0f, 1.0f,
            -0.5f, -0.5f, -0.5f, -1.0f, 0.0f,  0.0f,  0.0f, 1.0f, -0.5f, -0.5f, -0.5f, -1.0f, 0.0f,  0.0f,  0.0f, 1.0f,
            -0.5f, -0.5f, 0.5f,  -1.0f, 0.0f,  0.0f,  0.0f, 0.0f, -0.5f, 0.5f,  0.5f,  -1.0f, 0.0f,  0.0f,  1.0f, 0.0f,

            0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 0.5f,  0.5f,  -0.5f, 1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
            0.5f,  -0.5f, -0.5f, 1.0f,  0.0f,  0.0f,  0.0f, 1.0f, 0.5f,  -0.5f, -0.5f, 1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
            0.5f,  -0.5f, 0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f, 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

            -0.5f, -0.5f, -0.5f, 0.0f,  -1.0f, 0.0f,  0.0f, 1.0f, 0.5f,  -0.5f, -0.5f, 0.0f,  -1.0f, 0.0f,  1.0f, 1.0f,
            0.5f,  -0.5f, 0.5f,  0.0f,  -1.0f, 0.0f,  1.0f, 0.0f, 0.5f,  -0.5f, 0.5f,  0.0f,  -1.0f, 0.0f,  1.0f, 0.0f,
            -0.5f, -0.5f, 0.5f,  0.0f,  -1.0f, 0.0f,  0.0f, 0.0f, -0.5f, -0.5f, -0.5f, 0.0f,  -1.0f, 0.0f,  0.0f, 1.0f,

            -0.5f, 0.5f,  -0.5f, 0.0f,  1.0f,  0.0f,  0.0f, 1.0f, 0.5f,  0.5f,  -0.5f, 0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
            0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f, 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
            -0.5f, 0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f, -0.5f, 0.5f,  -0.5f, 0.0f,  1.0f,  0.0f,  0.0f, 1.0f};

        // Coordinate.
        glm::vec3 cube_positions[] = {glm::vec3(0.0f, 0.0f, 0.0f),    glm::vec3(2.0f, 5.0f, -15.0f),
                                      glm::vec3(-1.5f, -2.2f, -2.5f), glm::vec3(-3.8f, -2.0f, -12.3f),
                                      glm::vec3(2.4f, -0.4f, -3.5f),  glm::vec3(-1.7f, 3.0f, -7.5f),
                                      glm::vec3(1.3f, -2.0f, -2.5f),  glm::vec3(1.5f, 2.0f, -2.5f),
                                      glm::vec3(1.5f, 0.2f, -1.5f),   glm::vec3(-1.3f, 1.0f, -1.5f)};

        // Point light positions.
        glm::vec3 pointLightPositions[] = {glm::vec3(0.7f, 0.2f, 2.0f), glm::vec3(2.3f, -3.3f, -4.0f),
                                           glm::vec3(-4.0f, 2.0f, -12.0f), glm::vec3(0.0f, 0.0f, -3.0f)};

        // Box vertex buffer object.
        // The light and the box use the same vbo because they have same shape(box).
        unsigned int vbo = bindVertexBufferObject(vertices, sizeof(vertices));

        // Box vertex array object.
        unsigned int box_vao = bindVertexArrayObject();
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        // Attributes.
        // Vertex positions.
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        // Normals.
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);
        // Texture coordinates.
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
        glEnableVertexAttribArray(2);

        // Light vertex array object.
        unsigned int light_vao = bindVertexArrayObject();
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        // Attributes.
        // Vertex positions.
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        // Texture.
        unsigned int diffuse_texture =
            generateTexture("D:\\Turotials\\StudyOpenGL\\OpenGL\\Assets\\container2.png", GL_TEXTURE0, GL_RGBA);
        unsigned int specular_texture = generateTexture(
            "D:\\Turotials\\StudyOpenGL\\OpenGL\\Assets\\container2_specular.png", GL_TEXTURE1, GL_RGBA);
        // unsigned int emission_texture = generateTexture(
        //     "D:\\Turotials\\StudyOpenGL\\OpenGL\\Assets\\matrix.jpg", GL_TEXTURE2, GL_RGB);

        // Compile.
        Shader box_shader("D:/Turotials/StudyOpenGL/OpenGL/OpenGL/lighting/box_shader.vs",
                          "D:/Turotials/StudyOpenGL/OpenGL/OpenGL/lighting/box_shader.fs");
        Shader cube_lamp_shader("D:/Turotials/StudyOpenGL/OpenGL/OpenGL/lighting/lamp_shader.vs",
                                "D:/Turotials/StudyOpenGL/OpenGL/OpenGL/lighting/lamp_shader.fs");

        glm::mat4 projection(1.0f);
        projection = glm::perspective(glm::radians(60.0f), (float)(640.0 / 480.0), 0.1f, 500.0f);

        glEnable(GL_DEPTH_TEST);
        // Capture the mouse in the window.
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        // Set call back function to process mouse movement.
        glfwSetCursorPosCallback(window, processMouseMovement);
        // Set call back function to process mouse scroll.
        glfwSetScrollCallback(window, processMouseScroll);

        // Use the box shader.
        box_shader.use();
        // Set textures.
        box_shader.setInt("material.diffuse", 0);
        box_shader.setInt("material.specular", 1);
        // box_shader.setInt("material.emission", 2);

        // Direction light.
        box_shader.setVec3("dirLight.direction", -0.2f, -1.0f, -0.3f);
        box_shader.setVec3("dirLight.ambient", 0.05f, 0.05f, 0.05f);
        box_shader.setVec3("dirLight.diffuse", 0.4f, 0.4f, 0.4f);
        box_shader.setVec3("dirLight.specular", 0.5f, 0.5f, 0.5f);
        // Point lights.
        for (int i = 0; i < 4; ++i) {
            std::string pointLightName = std::string("pointLights[") + (char)(i + '0') + ']';
            box_shader.setVec3(pointLightName + ".position", pointLightPositions[i]);

            box_shader.setVec3(pointLightName + ".ambient", 0.2f, 0.2f, 0.2f);
            box_shader.setVec3(pointLightName + ".diffuse", 0.5f, 0.5f, 0.5f);
            box_shader.setVec3(pointLightName + ".specular", 1.0f, 1.0f, 1.0f);

            box_shader.setFloat(pointLightName + ".conatant", 1.0f);
            box_shader.setFloat(pointLightName + ".linear", 0.09f);
            box_shader.setFloat(pointLightName + ".quadratic", 0.032f);
        }
        // Spot light.
        box_shader.setVec3("spotLight.basic.ambient", 0.0f, 0.0f, 0.0f);
        box_shader.setVec3("spotLight.basic.diffuse", 1.0f, 1.0f, 1.0f);
        box_shader.setVec3("spotLight.basic.specular", 1.0f, 1.0f, 1.0f);
        box_shader.setFloat("spotLight.basic.constant", 1.0f);
        box_shader.setFloat("spotLight.basic.linear", 0.09f);
        box_shader.setFloat("spotLight.basic.quadratic", 0.032f);

        box_shader.setFloat("spotLight.cutOff", glm::cos(glm::radians(12.5f)));
        box_shader.setFloat("spotLight.outerCutOff", glm::cos(glm::radians(15.0f)));

        // Light damping.

        /* Loop until the user closes the window */
        while (!glfwWindowShouldClose(window)) {
            /* Render here */
            glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            float current_frame = glfwGetTime();
            delta_time = current_frame - last_frame;
            last_frame = current_frame;

            processKeyboard(window);

            //// Rotate light.
            // lightPos.x = 2.0f * sin(current_frame) + 1.0f;
            // lightPos.y = sin(current_frame / 2.0f);

            //// Change color.
            // glm::vec3 lightColor;
            // lightColor.x = sin(current_frame * 2.0f);
            // lightColor.y = sin(current_frame * 0.7f);
            // lightColor.z = sin(current_frame * 1.3f);

            // glm::vec3 diffuseColor = lightColor * glm::vec3(0.5f);    // 降低影响
            // glm::vec3 ambientColor = diffuseColor * glm::vec3(0.2f);  // 很低的影响

            // Camera.
            glm::mat4 view = camera.getViewMatrix();
            glm::mat4 model(1.0f);
            // Use the box shader.
            box_shader.use();
            // Set coordinates.
            // box_shader.setMat4("model", model);
            box_shader.setMat4("projection", projection);
            // Set materials.
            box_shader.setVec3("material.specular", 0.5f, 0.5f, 0.5f);
            box_shader.setFloat("material.shininess", 64.0f);
            // Set the view matrix.
            box_shader.setMat4("view", view);
            box_shader.setVec3("viewPos", camera.position_);

            // Set spot light coordinates.
            box_shader.setVec3("spotLight.basic.position", camera.position_);
            box_shader.setVec3("spotLight.direction", camera.front_);

            // Rotate boxes.
            for (unsigned int i = 0; i < 10; ++i) {
                glm::mat4 model(1.0f);
                model = glm::translate(model, cube_positions[i]);
                float angle = 20.0f * i;
                model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
                model = glm::rotate(model, (float)glfwGetTime() * glm::radians(20.0f), glm::vec3(0.5f, 1.0f, 0.0f));
                box_shader.setMat4("model", model);
                // Draw the box.
                glBindVertexArray(box_vao);
                glDrawArrays(GL_TRIANGLES, 0, 36);
            }

            // Use the lamp shader.
            cube_lamp_shader.use();
            // Set the view matrix.
            cube_lamp_shader.setMat4("view", view);
            // Set the projection matrix.
            cube_lamp_shader.setMat4("projection", projection);
            for (int i = 0; i < 4; ++i) {
                model = glm::mat4(1.0f);
                model = glm::translate(model, pointLightPositions[i]);
                model = glm::scale(model, glm::vec3(0.2f));
                cube_lamp_shader.setMat4("model", model);
                // Draw the lamp.
                glBindVertexArray(light_vao);
                glDrawArrays(GL_TRIANGLES, 0, 36);
            }

            /* Swap front and back buffers */
            glfwSwapBuffers(window);

            /* Poll for and process events */
            glfwPollEvents();
        }

        // Optianl, de-allocate all resource once ther've outlived their purpose.
        glDeleteVertexArrays(1, &box_vao);
        glDeleteVertexArrays(1, &light_vao);
        glDeleteBuffers(1, &vbo);
    }
}  // namespace Lighting

namespace BasicModel {
    void drawModel(GLFWwindow* window)
    {
        // Flip y-axis of loaded texture.
        stbi_set_flip_vertically_on_load(true);

        // Shader.
        Shader modelShader("D:/Turotials/StudyOpenGL/OpenGL/OpenGL/model/model.vs",
                           "D:/Turotials/StudyOpenGL/OpenGL/OpenGL/model/model.fs");
        
        // Model.
        Model modeler("D:/Turotials/StudyOpenGL/OpenGL/Assets/backpack.obj");

        glEnable(GL_DEPTH_TEST);
        // Capture the mouse in the window.
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        // Set call back function to process mouse movement.
        glfwSetCursorPosCallback(window, processMouseMovement);
        // Set call back function to process mouse scroll.
        glfwSetScrollCallback(window, processMouseScroll);
        
        /* Loop until the user closes the window */
        while (!glfwWindowShouldClose(window)) {
            /* Render here */
            glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            float current_frame = glfwGetTime();
            delta_time = current_frame - last_frame;
            last_frame = current_frame;

            processKeyboard(window);

            modelShader.use();
            // view/projection transformations
            glm::mat4 projection =
                glm::perspective(glm::radians(camera.zoom_), 640.0f / 480.0f, 0.1f, 100.0f);
            glm::mat4 view = camera.getViewMatrix();
            modelShader.setMat4("projection", projection);
            modelShader.setMat4("view", view);

            // render the loaded model
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(
                model, glm::vec3(0.0f, 0.0f, 0.0f));             // translate it down so it's at the center of the scene
            model =
                glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));  // it's a bit too big for our scene, so scale it down
            modelShader.setMat4("model", model);
            modeler.draw(modelShader);

            /* Swap front and back buffers */
            glfwSwapBuffers(window);

            /* Poll for and process events */
            glfwPollEvents();
        }
    }
}  // namespace Model

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

    // float vertices[] = {-0.5f, -0.5f, 0.0f, 0.5f, -0.5f, 0.0f, 0.0f, 0.5f, 0.0f};
    // drawTriangle(window, vertices, sizeof(vertices));

    // GettingStarted::drawBox(window);
    //Lighting::drawBox(window);
    BasicModel::drawModel(window);

    glfwTerminate();
    return 0;
}
