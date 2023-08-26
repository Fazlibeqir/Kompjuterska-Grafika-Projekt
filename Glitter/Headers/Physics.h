//
// Created by Elena on 21.7.2023.
//
// Modified by Beqir on 22/08/2023.
//

#ifndef KOMPJUTERSKA_GRAFIKA_PROJEKT_PHYSICS_H
#define KOMPJUTERSKA_GRAFIKA_PROJEKT_PHYSICS_H
#pragma once

#include <btBulletDynamicsCommon.h>
#include <glm/glm.hpp>
#include "GlobalVariables.h"

enum shapes {
    BOX, SPHERE, CYLINDER
};
enum collflag {
    COLL_TERRAIN = 1,
    COLL_CHASSIS = 1 << 1,
    COLL_TYRE = 1 << 2,
    COLL_CAR = COLL_CHASSIS | COLL_TYRE,
    COLL_EVERYTHING = -1
};
class Physics {
public:
    btDiscreteDynamicsWorld *dynamicsWorld; // the main physical simulation class
    btAlignedObjectArray<btCollisionShape *> collisionShapes; // vector for all the Collision Shapes of the scene
    btDefaultCollisionConfiguration *collisionConfiguration; // setup for the collision manager
    btCollisionDispatcher *dispatcher; // collision manager
    btBroadphaseInterface *overlappingPairCache; // method for the broadphase collision detection
    btSequentialImpulseConstraintSolver *solver; // constraints solver

    btRigidBody *car;
    btRigidBody *t1, *t2, *t3, *t4;
    btGeneric6DofSpringConstraint *c1, *c2, *c3, *c4;
    glm::vec3 plane_pos[GlobalVariables::tiles];

    Physics();
    glm::vec3 getPlayerPosition();
    btRigidBody * createRigidBody(int type, glm::vec3 pos,
                                    glm::vec3 size,
                                    glm::vec3 rot,
                                    float m, float friction,
                                    float restitution,
                                    short group, short mask);
    void generateTerrain();
    void generateInvisibleWalls();
    void generateCamaro();
    void addConstraints();
    void updateMovements();
    ~Physics();

private:
    void accelerationMovement(btMatrix3x3 &rot, short &braking);
    void brakingMovement(btMatrix3x3 &rot, short &braking);
    void handbrakeMovement(btMatrix3x3 &rot, short &braking);
    void resetMovement(btMatrix3x3 &rot);
    void jumpMovement();
};

#endif //KOMPJUTERSKA_GRAFIKA_PROJEKT_PHYSICS_H
