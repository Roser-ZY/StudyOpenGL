#include "camera.h"
#include "glad/glad.h"
#include "shader.h"

#include <GLFW/glfw3.h>
#include <iostream>

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
    static float lastX = 400;
    static float lastY = 300;
    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos;       // The window coordinate is reversed with graphics.
    lastX = xpos;
    lastY = ypos;
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

    drawBox(window);

    glfwTerminate();
    return 0;
}
