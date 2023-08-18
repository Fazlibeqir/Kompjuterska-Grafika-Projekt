#pragma once

#include <vector>

#include <glad/glad.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

enum Camera_Movement {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT
};

const GLfloat YAW = -90.0f; // Y
const GLfloat PITCH = -10.0f; // X

const GLfloat SPEED = 3.0f;
const GLfloat SENSITIVITY = 0.1f;

class Camera {
public:
    // Camera Attributes
    glm::vec3 Position;
    glm::vec3 Front;
    glm::vec3 WorldFront;
    glm::vec3 Up; // camera local UP vector
    glm::vec3 Right;
    glm::vec3 WorldUp; // camera world UP vector -> needed for the initial computation of Right vector
    GLboolean onGround;

    //Euler Angles

    GLfloat Yaw;
    GLfloat Pitch;

    //Camera options

    GLfloat MovementSpeed;
    GLfloat MouseSensitivity;

    Camera(glm::vec3 position, GLboolean onGround) {
        this->Position = position;
        this->WorldUp = glm::vec3(0.0f, 1.0f, 0.0f);
        this->Yaw = YAW;
        this->Pitch = PITCH;
        this->onGround = onGround;
        this->MovementSpeed = SPEED;
        this->MouseSensitivity = SENSITIVITY;
        this->updateCameraVectors();
    }

    glm::mat4 GetViewMatrix() {
        return glm::lookAt(this->Position, this->Position + this->Front, this->Up);
    }

    void ProcessKeyboard(Camera_Movement direction, GLfloat deltaTime) {
        GLfloat velocity = this->MovementSpeed * deltaTime;
        if (direction == FORWARD)
            this->Position += (this->onGround ? this->WorldFront : this->Front) * velocity;
        if (direction == BACKWARD)
            this->Position -= (this->onGround ? this->WorldFront : this->Front) * velocity;
        if (direction == LEFT)
            this->Position -= this->Right * velocity;
        if (direction == RIGHT)
            this->Position += this->Right * velocity;
    }

    void ProcessMouseMovement(GLfloat xoffset, GLfloat yoffset, GLboolean constraintPitch = GL_TRUE) {
        xoffset *= this->MouseSensitivity;
        yoffset *= this->MouseSensitivity;

        this->Yaw += xoffset;
        this->Pitch += yoffset;

        if (constraintPitch) {
            if (this->Pitch > 89.0f)
                this->Pitch = 89.0f;
            if (this->Pitch < -89.0f)
                this->Pitch = -89.0f;
        }

        this->updateCameraVectors();
    }

    void LookAt(float x, float y, float z) {
        glm::vec2 dir(x, z);
        glm::vec2 dir2(y, z);
        dir = normalize(dir);
        dir2 = normalize(dir2);

        this->Yaw = glm::degrees(glm::atan(dir.y, dir.x));
        this->updateCameraVectors();
    }

private:

    void updateCameraVectors() {
        glm::vec3 front;
        front.x = cos(glm::radians(this->Yaw)) * cos(glm::radians(this->Pitch));
        front.y = sin(glm::radians(this->Pitch));
        front.z = sin(glm::radians(this->Yaw)) * cos(glm::radians(this->Pitch));
        this->WorldFront = this->Front = glm::normalize(front);
        this->WorldFront.y = 0.0f;
        this->Right = glm::normalize(glm::cross(this->Front, this->WorldUp));
        this->Up = glm::normalize(glm::cross(this->Right, this->Front));
    }
};
