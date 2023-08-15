//
// Created by Beqir on 12/08/2023.
//

#ifndef KOMPJUTERSKA_GRAFIKA_PROJEKT_CAR_HPP
#define KOMPJUTERSKA_GRAFIKA_PROJEKT_CAR_HPP


#include "glm/vec3.hpp"
#include "init.h"
class FrameBuffer;
class Car {
public:
    Car(FrameBuffer* frameBuffer,float mass, float  engineForce,float frictionCoefficient);

    // Methods for updating physics
    void updatePhysics(float deltatime);

    float getEngineForce() const;

    void setEngineForce(float engineForce);

    float getMaxEngineForce() const;

    void setMaxEngineForce(float maxEngineForce);

    float getSteeringInput() const;

    void setSteeringInput(float steeringInput);

    glm::vec3 getPosition();
    void setPosition(glm::vec3 position);

private:
    float mass;
    float engineForce;
    float maxEngineForce;
    float frictionCoefficient;

    glm::vec3 velocity;
    glm::vec3 acceleration;
    glm::vec3 position;

    static const float GRAVITY;

    float orientation;
    float angularVelocity;
    float angularAcceleration;

    float steeringInput;
    float steeringSensitivity;
    float momentOfInertia;

    FrameBuffer* framebuffer;
};


#endif //KOMPJUTERSKA_GRAFIKA_PROJEKT_CAR_HPP
