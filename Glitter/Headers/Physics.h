//
// Created by Elena on 21.7.2023.
//

#ifndef KOMPJUTERSKA_GRAFIKA_PROJEKT_PHYSICS_H
#define KOMPJUTERSKA_GRAFIKA_PROJEKT_PHYSICS_H
#pragma once

#include <btBulletDynamicsCommon.h>
#include <memory>
#include "glm/vec3.hpp"
#include "mesh.hpp"
#include "Model.hpp"

class Physics {
public:
    Physics(btDiscreteDynamicsWorld* dynamicsWorld, const std::string& modelPath);
    ~Physics();

    void initialize(const std::string modelPaths[]);
    void update(float deltaTime);
    glm::vec3 getCarPosition() const;
    void applyEngineForce(float force);
    void applyBrake(float brake);
    void applySteering(float steering);

    static btCollisionShape* createChassisCollisionShape(const std::vector<Vertex>& chassisVertices);
    static btCollisionShape* createWheelCollisionShape(const std::vector<Vertex>& wheelVertices);
    btRigidBody* createChassisRigidBody();
    btRigidBody* createWheelRigidBody();
    btHingeConstraint* createWheelConstraint();
private:
    btDiscreteDynamicsWorld* m_dynamicsWorld;
    btCollisionShape* chassisShape;
    btCollisionShape* wheelShape;

    btRigidBody* chassisRigidBody;
    btRigidBody* wheelRigidBody;

    btRigidBody* m_carChassis;
    btRigidBody* m_wheelRigidBodies[4];
};

#endif //KOMPJUTERSKA_GRAFIKA_PROJEKT_PHYSICS_H
