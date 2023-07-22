//
// Created by Beqir on 22.7.2023.
//

#ifndef KOMPJUTERSKA_GRAFIKA_PROJEKT_CARPHYSICS_HPP
#define KOMPJUTERSKA_GRAFIKA_PROJEKT_CARPHYSICS_HPP
#pragma once

#include <glm/glm.hpp>
#include <GLFW/glfw3.h>

class CarPhysics {
public:
    CarPhysics();
    void update(GLFWwindow* window, float deltaTime);
    void accelerate();
    void decelerate();
    void steerLeft();
    void steerRight();
    void reset();

    glm::vec3 getPosition() const;
    float getRotation() const;
    float getSpeed() const;
    float getAcceleration() const;
    float getMaxSpeed() const;
private:
    glm::vec3 position;
    glm::vec3 velocity;
    float rotation;
    float speed;
    float acceleration;
    float maxSpeed;
    float steeringAngle;
    const float maxSteeringAngle = 25.0f;
    const float maxAcceleration = 0.01f;
    const float maxSpeedLimit = 5.0f;
};


#endif //KOMPJUTERSKA_GRAFIKA_PROJEKT_CARPHYSICS_HPP
