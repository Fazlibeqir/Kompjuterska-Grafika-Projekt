//
// Created by Elena on 21.7.2023.
//

#include <vector>
#include "Physics.h"
#include "mesh.hpp"

Physics::Physics(btDiscreteDynamicsWorld* dynamicsWorld, const std::string& modelPath)
        : m_dynamicsWorld(dynamicsWorld) {
    // Initialize other member variables
}

Physics::~Physics() {
    // Clean up allocated resources
}

void Physics::initialize(const std::vector<std::string>& modelPaths) {
    // Initialize the car's rigidbodies and constraints
    // Implement this function to create the car physics components
    // Load the OBJ model
    for (const std::string& modelPath : modelPaths) {
    Model carModel(modelPath);

    // Separate vertices into chassis and wheel groups based on vertex groups
    std::vector<Vertex> chassisVertices = carModel.getChassisVertices();
    std::vector<Vertex> wheelVertices = carModel.getWheelVertices();

    // Create collision shapes and rigidbodies for chassis and wheels
    btCollisionShape* chassisShape = createChassisCollisionShape(chassisVertices);
    btCollisionShape* wheelShape = createWheelCollisionShape(wheelVertices);

    btRigidBody* chassisRigidBody = createChassisRigidBody(chassisShape);
    btRigidBody* wheelRigidBody = createWheelRigidBody(wheelShape);

    // Add the rigidbodies to the dynamics world
    m_dynamicsWorld->addRigidBody(chassisRigidBody);
    m_dynamicsWorld->addRigidBody(wheelRigidBody);

    // Add constraints to simulate connections between chassis and wheels
    btHingeConstraint* wheelConstraint = createWheelConstraint(chassisRigidBody, wheelRigidBody);

    m_dynamicsWorld->addConstraint(wheelConstraint, true);
    }
}

void Physics::update(float deltaTime) {
    // Update the car's physics simulation
    // Implement this function to update car physics based on deltaTime
}
// Inside Physics.cpp
glm::vec3 Physics::getCarPosition() const {
    // Implement this function to return the car's position
    // based on your physics simulation
    // For example, you might return the position of the car's rigidbody
    // Make sure to convert the physics engine's position to glm::vec3
    // and apply any necessary transformations if needed
    // Return the car's position as a glm::vec3
}
void Physics::applyEngineForce(float force) {
    // Apply engine force to the car's rigidbody
    // Implement this function to apply forces to the car
}

void Physics::applyBrake(float brake) {
    // Apply brake force to the car's rigidbody
    // Implement this function to apply brake forces
}

void Physics::applySteering(float steering) {
    // Apply steering torque to the car's rigidbody
    // Implement this function to apply steering torques
}
