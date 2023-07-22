//
// Created by Elena on 21.7.2023.
//

#include "Physics.h"

Physics::Physics() : collisionConfig(nullptr), dispatcher(nullptr), broadPhase(nullptr) {}

Physics::~Physics() {
    // Clean up Bullet components
    delete broadPhase;
    delete dispatcher;
    delete collisionConfig;
}

void Physics::initPhysics() {
    // Create the Bullet collision configuration
    collisionConfig = new btDefaultCollisionConfiguration();

    // Create the Bullet dispatcher
    dispatcher = new btCollisionDispatcher(collisionConfig);

    // Create the Bullet broadphase
    broadPhase = new btDbvtBroadphase();

    // Optional: You can create a constraint solver and a physics world if needed
    // btSequentialImpulseConstraintSolver* solver = new btSequentialImpulseConstraintSolver;
    // btDiscreteDynamicsWorld* dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher, broadPhase, solver, collisionConfig);

    // You can set additional properties and parameters for the physics world here if needed
}
void Physics::update(float deltaTime){

}
