#include "camera.h"

Camera::Camera(glm::vec3 position, glm::vec3 up, float yaw, float pitch)
    : position_(position)
    , world_up_(up)
    , front_(glm::vec3(0.0f, 0.0f, -1.0f))
    , yaw_(yaw)
    , pitch_(pitch)
    , movement_speed_(SPEED)
    , mouse_sensitivity_(SENSITIVITY)
    , zoom_(ZOOM)
{
    updateCameraVectors();
}

Camera::Camera(float pos_x, float pos_y, float pos_z, float up_x, float up_y, float up_z, float yaw, float pitch)
    : Camera(glm::vec3(pos_x, pos_y, pos_z), glm::vec3(up_x, up_y, up_z), yaw, pitch)
{
}

void Camera::processKeyboard(CameraMovement direction, float delta_time)
{
    float velocity = movement_speed_ * delta_time;
    if (direction == FORWARD)
        position_ += front_ * velocity;
    if (direction == BACKWARD)
        position_ -= front_ * velocity;
    if (direction == LEFT)
        position_ -= right_ * velocity;
    if (direction == RIGHT)
        position_ += right_ * velocity;
}

void Camera::processMouseMovement(float xoffset, float yoffset, GLboolean constrain_pitch)
{
    xoffset *= mouse_sensitivity_;
    yoffset *= mouse_sensitivity_;

    yaw_ += xoffset;
    pitch_ += yoffset;

    // make sure that when pitch is out of bounds, screen doesn't get flipped
    if (constrain_pitch) {
        if (pitch_ > 89.0f)
            pitch_ = 89.0f;
        if (pitch_ < -89.0f)
            pitch_ = -89.0f;
    }

    // update Front, Right and Up Vectors using the updated Euler angles
    updateCameraVectors();
}

void Camera::processMouseScroll(float yoffset)
{
    zoom_ -= (float)yoffset;
    if (zoom_ < 1.0f)
        zoom_ = 1.0f;
    if (zoom_ > 45.0f)
        zoom_ = 45.0f;
}

void Camera::updateCameraVectors()
{
    // Calculate the new front vector
    glm::vec3 front;
    front.x = cos(glm::radians(yaw_)) * cos(glm::radians(pitch_));
    front.y = sin(glm::radians(pitch_));
    front.z = sin(glm::radians(yaw_)) * cos(glm::radians(pitch_));
    front_ = glm::normalize(front);
    // Also re-calculate the Right and Up vector
    right_ = glm::normalize(
        glm::cross(front_, world_up_));  // Normalize the vectors, because their length gets closer to 0 the more you
                                         // look up or down which results in slower movement.
    up_ = glm::normalize(glm::cross(right_, front_));
}