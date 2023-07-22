//
// Created by Elena on 21.7.2023.
//

#ifndef KOMPJUTERSKA_GRAFIKA_PROJEKT_PHYSICS_H
#define KOMPJUTERSKA_GRAFIKA_PROJEKT_PHYSICS_H
#pragma once

#include <btBulletDynamicsCommon.h>
#include <memory>

class Physics {
public:
    Physics();
    ~Physics();

    void initPhysics();
    void update(float deltaTime);

private:
    btDefaultCollisionConfiguration* collisionConfig;
    btCollisionDispatcher* dispatcher;
    btBroadphaseInterface* broadPhase;
};

#endif //KOMPJUTERSKA_GRAFIKA_PROJEKT_PHYSICS_H
