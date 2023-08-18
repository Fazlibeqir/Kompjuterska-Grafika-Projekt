//
// Created by Elena on 21.7.2023.
//

#include <vector>
#include "Physics.h"

const float CHASSIS_MASS = 1500.0f; // Mass of the chassis in kilograms
const float WHEEL_MASS = 20.0f;     // Mass of each wheel in kilograms
const float BRAKE_STRENGTH = 1000.0f; // Adjust this value through testing
const float STEERING_SENSITIVITY = 500.0f; // Adjust this value through testing


Physics::Physics(btDiscreteDynamicsWorld* dynamicsWorld, const std::string& modelPath)
        : m_dynamicsWorld(dynamicsWorld) {
    // Initialize other member variables
}

Physics::~Physics() {
    // Remove and delete rigidbodies
    if (m_carChassis) {
        m_dynamicsWorld->removeRigidBody(m_carChassis);
        delete m_carChassis;
    }

    for (auto & m_wheelRigidBodie : m_wheelRigidBodies) {
        if (m_wheelRigidBodie) {
            m_dynamicsWorld->removeRigidBody(m_wheelRigidBodie);
            delete m_wheelRigidBodie;
        }
    }

    // Delete collision shapes
    if (chassisShape) {
        delete chassisShape;
    }
    if (wheelShape) {
        delete wheelShape;
    }

    // Delete the dynamics world
    if (m_dynamicsWorld) {
        delete m_dynamicsWorld;
    }
}

void Physics::initialize(const std::string modelPaths[]) {
    // Initialize the car's rigidbodies and constraints
    // Implement this function to create the car physics components

    // Load the OBJ model
    for (int i = 0; i < 2; i++) {
        Model carModel(modelPaths[i]);

        // Separate vertices into chassis and wheel groups based on vertex groups
        const std::vector<Vertex>& chassisVertices = carModel.GetChassisVertices();
        const std::vector<Vertex>& wheelVertices = carModel.GetWheelVertices();

        // Create collision shapes and rigidbodies for chassis and wheels
        chassisShape = createChassisCollisionShape(chassisVertices); // Implement createChassisCollisionShape
        wheelShape = createWheelCollisionShape(wheelVertices); // Implement createWheelCollisionShape

        chassisRigidBody = createChassisRigidBody(); // Implement createChassisRigidBody
        wheelRigidBody = createWheelRigidBody(); // Implement createWheelRigidBody

        // Add the rigidbodies to the dynamics world
        if (m_dynamicsWorld) {
            m_dynamicsWorld->addRigidBody(chassisRigidBody);
            m_dynamicsWorld->addRigidBody(wheelRigidBody);
        }

        // Add constraints to simulate connections between chassis and wheels
        btHingeConstraint* wheelConstraint = createWheelConstraint(); // Implement createWheelConstraint

        if (m_dynamicsWorld && wheelConstraint) {
            m_dynamicsWorld->addConstraint(wheelConstraint, true);
        }
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
    btMotionState* motionState = chassisRigidBody->getMotionState();
    if (motionState) {
        btTransform transform;
        motionState->getWorldTransform(transform);

        // Extract position from the transform and convert to glm::vec3
        btVector3 position = transform.getOrigin();
        glm::vec3 carPosition(position.getX(), position.getY(), position.getZ());

        return carPosition;
    } else {
        // Return a default position or handle the error case
        return {0.0f, 0.0f, 0.0f};
    }
}
void Physics::applyEngineForce(float force) {
    // Apply engine force to the car's rigidbody
    // Implement this function to apply forces to the car
    chassisRigidBody->applyCentralForce(btVector3(0.0f, 0.0f, force));
}

void Physics::applyBrake(float brake) {
    // Apply brake force to the car's rigidbody
    // Implement this function to apply brake forces
    float brakeForce = -brake * BRAKE_STRENGTH; // Adjust BRAKE_STRENGTH as needed
    chassisRigidBody->applyCentralForce(btVector3(0.0f, 0.0f, brakeForce));
}

void Physics::applySteering(float steering) {
    // Apply steering torque to the car's rigidbody
    // Implement this function to apply steering torques
    float steeringTorque = steering * STEERING_SENSITIVITY; // Adjust STEERING_SENSITIVITY as needed
    chassisRigidBody->applyTorque(btVector3(0.0f, steeringTorque, 0.0f));
}

btCollisionShape* Physics::createChassisCollisionShape(const std::vector<Vertex>& chassisVertices) {
    btCompoundShape* compoundShape = new btCompoundShape();

    // Loop through chassisVertices and add simpler collision shapes
    // (boxes, cylinders, spheres, etc.) to the compoundShape
    // based on the vertices of your car model

    // Example: Adding a box shape
    btBoxShape* boxShape = new btBoxShape(btVector3(1.0, 0.5, 2.0));
    btTransform localTransform;
    localTransform.setIdentity();
    compoundShape->addChildShape(localTransform, boxShape);

    return compoundShape;
}

btCollisionShape* Physics::createWheelCollisionShape(const std::vector<Vertex>& wheelVertices) {
    // Create and return a cylinder or sphere shape based on your preference
    btCollisionShape* wheelShape = new btCylinderShape(btVector3(0.5, 0.5, 0.2));
    return wheelShape;
}

btRigidBody* Physics::createChassisRigidBody() {
    btTransform startTransform;
    startTransform.setIdentity();

    btVector3 localInertia(0, 0, 0);
    bool isDynamic = (CHASSIS_MASS > 0.0f);
    if (isDynamic)
        chassisShape->calculateLocalInertia(CHASSIS_MASS, localInertia);

    startTransform.setOrigin(btVector3(0, 2, 0)); // Adjust the initial position

    btDefaultMotionState* motionState = new btDefaultMotionState(startTransform);
    btRigidBody::btRigidBodyConstructionInfo rbInfo(CHASSIS_MASS, motionState, chassisShape, localInertia);
    btRigidBody* newChassisRigidBody = new btRigidBody(rbInfo);

    // Set friction, damping, restitution, etc. for the chassisRigidBody

    return chassisRigidBody;
}

btRigidBody* Physics::createWheelRigidBody() {
    btTransform startTransform;
    startTransform.setIdentity();

    btVector3 localInertia(0, 0, 0);
    wheelShape->calculateLocalInertia(WHEEL_MASS, localInertia);

    startTransform.setOrigin(btVector3(0, 0, 0)); // Adjust the initial position

    btDefaultMotionState* motionState = new btDefaultMotionState(startTransform);
    btRigidBody::btRigidBodyConstructionInfo rbInfo(WHEEL_MASS, motionState, wheelShape, localInertia);
    btRigidBody* newWheelRigidBody = new btRigidBody(rbInfo);

    // Set friction, damping, restitution, etc. for the wheelRigidBody

    return wheelRigidBody;
}

btHingeConstraint* Physics::createWheelConstraint() {
    // Define the pivot point for the hinge joint
    btVector3 pivotInA(0.0f, 0.0f, 0.0f);  // Center of chassis
    btVector3 pivotInB(0.0f, 0.0f, 0.0f);  // Center of wheel

    // Define the axis of rotation for the hinge joint
    btVector3 axisInA(0.0f, 1.0f, 0.0f);   // Vertical axis (y-axis) of chassis
    btVector3 axisInB(0.0f, 1.0f, 0.0f);   // Vertical axis (y-axis) of wheel

    // Create the hinge constraint
    btHingeConstraint* hingeConstraint = new btHingeConstraint(
            *chassisRigidBody, *wheelRigidBody,
            pivotInA, pivotInB,
            axisInA, axisInB,
            true
    );

    // Set constraint limits (if desired)
    hingeConstraint->setLimit(-SIMD_PI * 0.25f, SIMD_PI * 0.25f);

    return hingeConstraint;
}

