//
// Created by Elena on 21.7.2023.
//

#ifndef KOMPJUTERSKA_GRAFIKA_PROJEKT_PHYSICS_H
#define KOMPJUTERSKA_GRAFIKA_PROJEKT_PHYSICS_H
#pragma once

#include <btBulletDynamicsCommon.h>
#include <glm/glm.hpp>

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

    Physics() {
        this->collisionConfiguration = new btDefaultCollisionConfiguration();

        // default collision dispatcher (collision detection method)
        this->dispatcher = new btCollisionDispatcher(this->collisionConfiguration);
        this->overlappingPairCache = new btDbvtBroadphase();
        this->solver = new btSequentialImpulseConstraintSolver();

        // DynamicsWorld (main class for the physical simulation)
        this->dynamicsWorld = new btDiscreteDynamicsWorld(this->dispatcher, this->overlappingPairCache, this->solver,
                                                          this->collisionConfiguration);

        // set the gravity force
        this->dynamicsWorld->setGravity(btVector3(0.0f, -9.82f, 0.0f));
    }
    btRigidBody *
    createRigidBody(int type, glm::vec3 pos, glm::vec3 size, glm::vec3 rot, float m, float friction, float restitution,
                    short group, short mask) {

        btCollisionShape *cShape = NULL;
        btVector3 position = btVector3(pos.x, pos.y, pos.z);
        btQuaternion rotation;
        rotation.setEuler(rot.x, rot.y, rot.z);

        // Box Collision shape
        if (type == BOX) {
            btVector3 dim = btVector3(size.x, size.y, size.z);
            cShape = new btBoxShape(dim);
        }
            // Sphere Collision Shape
        else if (type == SPHERE) {
            cShape = new btSphereShape(size.x);
        } else if (type == CYLINDER) {
            btVector3 dim = btVector3(size.x, size.y, size.z);
            cShape = new btCylinderShape(dim);
        }

        // add this Collision Shape to the vector
        this->collisionShapes.push_back(cShape);

        // set the initial transformations
        btTransform objTransform;
        objTransform.setIdentity();
        objTransform.setRotation(rotation);
        objTransform.setOrigin(position);

        // if object has mass = 0 -> then it is static
        btScalar mass = m;
        bool isDynamic = (mass != 0.0f);

        // if it is dynamic (mass > 0) then calculate local inertia
        btVector3 localInertia(0.0f, 0.0f, 0.0f);
        if (isDynamic)
            cShape->calculateLocalInertia(mass, localInertia);

        btDefaultMotionState *motionState = new btDefaultMotionState(objTransform);

        btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, motionState, cShape, localInertia);
        rbInfo.m_friction = friction;
        rbInfo.m_restitution = restitution;

        if (type == SPHERE) {
            rbInfo.m_angularDamping = 0.3f;
            rbInfo.m_rollingFriction = 0.3f;
        } else if (type == CYLINDER) {
            rbInfo.m_angularDamping = 0.25f;
            rbInfo.m_rollingFriction = 0.75f;
        }

        // rigid body creation
        btRigidBody *body = new btRigidBody(rbInfo);
        this->dynamicsWorld->addRigidBody(body, group, mask);
        return body;
    }
    void Clear() {

        for (int i = this->dynamicsWorld->getNumCollisionObjects() - 1; i >= 0; i--) {
            btCollisionObject *obj = this->dynamicsWorld->getCollisionObjectArray()[i];
            btRigidBody *body = btRigidBody::upcast(obj);
            if (body && body->getMotionState()) {
                delete body->getMotionState();
            }
            this->dynamicsWorld->removeCollisionObject(obj);
            delete obj;
        }

        // remove all the Collision Shapes
        for (int j = 0; j < this->collisionShapes.size(); j++) {
            btCollisionShape *shape = this->collisionShapes[j];
            this->collisionShapes[j] = 0;
            delete shape;
        }

        // delete dynamics world
        delete this->dynamicsWorld;

        // delete solver
        delete this->solver;

        // delete broadphase
        delete this->overlappingPairCache;

        // delete dispatcher
        delete this->dispatcher;

        delete this->collisionConfiguration;

        this->collisionShapes.clear();
    }

};

#endif //KOMPJUTERSKA_GRAFIKA_PROJEKT_PHYSICS_H
