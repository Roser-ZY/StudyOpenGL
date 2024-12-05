#pragma once
#ifndef CAMERA_H
#define CAMERA_H

#include <glad/glad.h>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>

enum CameraMovement {
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT,
    UP,
    DOWN
};

const float YAW = -90.0f;
const float PITCH = 0.0f;
const float SPEED = 2.5f;
const float SENSITIVITY = 0.1f;
const float ZOOM = 45.0f;

class Camera {
public:
    // Camera attributes.
    glm::vec3 position_;
    glm::vec3 front_;
    glm::vec3 up_;
    glm::vec3 right_;
    glm::vec3 world_up_;
    // Euler angles.
    float yaw_;
    float pitch_;
    float movement_speed_;
    float mouse_sensitivity_;
    float zoom_;

    Camera(glm::vec3 position = glm::vec3(0.5f, 0.5f, 4.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f),
           float yaw = YAW, float pitch = PITCH);
    Camera(float pos_x, float pos_y, float pos_z, float up_x, float up_y, float up_z, float yaw, float pitch);

    glm::mat4 getViewMatrix()
    {
        return glm::lookAt(position_, position_ + front_, up_);
    }

    void processKeyboard(CameraMovement direction, float delta_time);
    void processMouseMovement(float xoffset, float yoffset, GLboolean constrain_pitch = true);
    void processMouseScroll(float yoffset);

private:
    void updateCameraVectors();
};

#endif
