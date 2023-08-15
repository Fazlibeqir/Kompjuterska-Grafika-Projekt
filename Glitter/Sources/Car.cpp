//
// Created by Beqir on 12/08/2023.
//

#include <glm/glm.hpp>
#include "Car.hpp"
#include "FrameBuffer.hpp"

const float Car::GRAVITY=9.81f;
Car::Car(FrameBuffer* framebuffer, float mass, float  engineForce,float frictionCoefficient):
        mass(mass),
        engineForce(engineForce),
        maxEngineForce(20000.0f),
        frictionCoefficient(frictionCoefficient),
        velocity(0.0f),
        acceleration(0.0f),
        position(carPosition),
        orientation(0.0f),
        angularVelocity(0.0f),
        angularAcceleration(0.0f),
        steeringInput(0.0f),
        steeringSensitivity(0.1f),
        momentOfInertia(3000.0f),
        framebuffer(framebuffer)
{

}

void Car::updatePhysics(float deltaTime1) {
    // Implement physics calculations here
    glm::vec3 netForce=glm::vec3 (0.0f,-mass* GRAVITY,0.0f);
    netForce+= engineForce*glm::vec3(glm::cos(orientation),0.0f,glm::sin(orientation));
    glm::vec3 friction= -frictionCoefficient*velocity;
    netForce+=friction;

    acceleration=netForce/mass;
    velocity+=acceleration*deltaTime1;

    position+=velocity*deltaTime1;
    orientation+= angularVelocity*deltaTime1;

    float steeringTorque= steeringInput* steeringSensitivity;
    angularAcceleration=steeringTorque/momentOfInertia;

    angularVelocity+=angularAcceleration*deltaTime1;
    orientation+=angularVelocity*deltaTime1;

    glm::vec3 oldPosition = position;
    position+= glm::vec3 (0.0f,0.0f,engineForce*deltaTime);

    float terrainHeight= framebuffer->getHeightFromHeightMap(position.x,position.z);
    if(position.y<terrainHeight){
        position.y=terrainHeight;
        // Calculate new velocity after collision
        glm::vec3 collisionNormal = glm::normalize(position - oldPosition);
        velocity = glm::reflect(velocity, collisionNormal) * 0.5f; // Reflect and dampen

        // Reset angular velocity and orientation
        angularVelocity = 0.0f;
        orientation = 0.0f;
    }
}
