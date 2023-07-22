//
// Created by Beqir on 22.7.2023.
//

#include "CarPhysics.hpp"


CarPhysics::CarPhysics()
        : position(glm::vec3(0.0f)), velocity(glm::vec3(0.0f)),
          rotation(0.0f), speed(0.0f), acceleration(0.0f),
          maxSpeed(0.02f), steeringAngle(0.0f) {}

void CarPhysics::update(GLFWwindow* window, float deltaTime)
{
    // Car movement and rotation based on user input
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        accelerate();
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        decelerate();
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        steerLeft();
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        steerRight();

    // Apply physics to update position and velocity
    position += velocity * deltaTime;
    velocity *= 0.95f; // Damping to simulate friction and air resistance

    // Clamp speed
    speed = glm::length(velocity);
    if (speed > maxSpeed)
    {
        velocity = glm::normalize(velocity) * maxSpeed;
        speed = maxSpeed;
    }

    // Limit acceleration
    if (acceleration > maxAcceleration)
        acceleration = maxAcceleration;
    else if (acceleration < -maxAcceleration)
        acceleration = -maxAcceleration;

    // Update rotation based on steering angle
    rotation -= steeringAngle * speed * deltaTime;

    // Clamp steering angle
    if (steeringAngle > maxSteeringAngle)
        steeringAngle = maxSteeringAngle;
    else if (steeringAngle < -maxSteeringAngle)
        steeringAngle = -maxSteeringAngle;
}

void CarPhysics::accelerate()
{
    acceleration += 0.001f;
}

void CarPhysics::decelerate()
{
    acceleration -= 0.001f;
}

void CarPhysics::steerLeft()
{
    steeringAngle += 0.5f;
}

void CarPhysics::steerRight()
{
    steeringAngle -= 0.5f;
}

void CarPhysics::reset()
{
    position = glm::vec3(0.0f);
    velocity = glm::vec3(0.0f);
    rotation = 0.0f;
    speed = 0.0f;
    acceleration = 0.0f;
    steeringAngle = 0.0f;
}

glm::vec3 CarPhysics::getPosition() const
{
    return position;
}

float CarPhysics::getRotation() const
{
    return rotation;
}

float CarPhysics::getSpeed() const
{
    return speed;
}

float CarPhysics::getAcceleration() const
{
    return acceleration;
}

float CarPhysics::getMaxSpeed() const
{
    return maxSpeed;
}
