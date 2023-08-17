//
// Created by Elena on 21.7.2023.
//

#ifndef KOMPJUTERSKA_GRAFIKA_PROJEKT_PHYSICS_H
#define KOMPJUTERSKA_GRAFIKA_PROJEKT_PHYSICS_H
#pragma once

#include <btBulletDynamicsCommon.h>
#include <memory>
#include "glm/vec3.hpp"

class Physics {
public:
    Physics(btDiscreteDynamicsWorld* dynamicsWorld, const std::string& modelPath);
    ~Physics();

    void initialize(const std::vector<std::string>& modelPaths);
    void update(float deltaTime);
    glm::vec3 getCarPosition() const;
    void applyEngineForce(float force);
    void applyBrake(float brake);
    void applySteering(float steering);

private:
    btDiscreteDynamicsWorld* m_dynamicsWorld;
    btRigidBody* m_carChassis;
    btRigidBody* m_wheelRigidBodies[4];
};

#endif //KOMPJUTERSKA_GRAFIKA_PROJEKT_PHYSICS_H
