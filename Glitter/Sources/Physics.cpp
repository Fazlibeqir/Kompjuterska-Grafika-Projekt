//
// Created by Elena on 19.8.2023.
//
#include "Physics.h"

Physics::Physics(){
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

btRigidBody * Physics::createRigidBody(int type, glm::vec3 pos, glm::vec3 size, glm::vec3 rot, float m, float friction, float restitution,
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

Physics::~Physics() {
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

void Physics:: generateTerrain(){
    btRigidBody *plane[GlobalVariables::tiles];
    glm::vec3 plane_size[GlobalVariables::tiles];
    for (unsigned int i = 0; i < GlobalVariables::grid_width; i++) {
        for (unsigned int j = 0; j < GlobalVariables::grid_height; j++) {
            plane_pos[i * (GlobalVariables::grid_height) + j] = glm::vec3(2 * GlobalVariables::plane_edge * i - GlobalVariables::plane_edge * (GlobalVariables::grid_width - 1),
                                                                          -0.0f * (i * (GlobalVariables::grid_height) + j),
                                                                          2 * GlobalVariables::plane_edge * j - GlobalVariables::plane_edge * (GlobalVariables::grid_height - 1));
            plane_size[i * (GlobalVariables::grid_height) + j] = glm::vec3(GlobalVariables::plane_edge, 0.0f, GlobalVariables::plane_edge);
            glm::vec3 plane_rot = glm::vec3(0.0f, 0.0f, 0.0f);

            if (GlobalVariables::track[j][i] == 0) {
                // Grass
                plane[i * (GlobalVariables::grid_height) + j] = createRigidBody(BOX, plane_pos[i * (GlobalVariables::grid_height) + j],
                                                                                           plane_size[i * (GlobalVariables::grid_height) + j], plane_rot,
                                                                                           0.0f, 0.25f, 0.25f, COLL_TERRAIN,
                                                                                           COLL_EVERYTHING);
            } else if (GlobalVariables::track[j][i] == 1) {
                // Asphalt
                plane[i * (GlobalVariables::grid_height) + j] = createRigidBody(BOX, plane_pos[i * (GlobalVariables::grid_height) + j] +
                                                                                                glm::vec3(0.0f, 0.05f, 0.0f),
                                                                                           plane_size[i * (GlobalVariables::grid_height) + j] +
                                                                                           glm::vec3(0.0f, 0.05f, 0.0f), plane_rot, 0.0f,
                                                                                           0.5f, 0.5f, COLL_TERRAIN, COLL_EVERYTHING);
            }

        }
    }
}
void Physics:: generateInvisibleWalls(){
    // Invisible walls
    float side;
    glm::vec3 wall_pos;
    glm::vec3 wall_size;
    btRigidBody *wall;

    side = GlobalVariables::plane_edge * GlobalVariables::grid_height;
    wall_size = glm::vec3(2 * side, 5.0f, 0.0f);

    wall_pos = glm::vec3(0.0f, 2.5f, -side);
    wall = createRigidBody(BOX, wall_pos, wall_size, glm::vec3(0.0f, 0.0f, 0.0f), 0.0f, 0.0f, 0.0f,
                                      COLL_TERRAIN, COLL_EVERYTHING);
    wall_pos = glm::vec3(0.0f, 2.5f, side);
    wall = createRigidBody(BOX, wall_pos, wall_size, glm::vec3(0.0f, 0.0f, 0.0f), 0.0f, 0.0f, 0.0f,
                                      COLL_TERRAIN, COLL_EVERYTHING);

    side = GlobalVariables::plane_edge * GlobalVariables::grid_width;
    wall_size = glm::vec3(0.0f, 5.0f, 2 * side);

    wall_pos = glm::vec3(-side, 2.5f, 0.0f);
    wall = createRigidBody(BOX, wall_pos, wall_size, glm::vec3(0.0f, 0.0f, 0.0f), 0.0f, 0.0f, 0.0f,
                                      COLL_TERRAIN, COLL_EVERYTHING);
    wall_pos = glm::vec3(side, 2.5f, 0.0f);
    wall = createRigidBody(BOX, wall_pos, wall_size, glm::vec3(0.0f, 0.0f, 0.0f), 0.0f, 0.0f, 0.0f,
                                      COLL_TERRAIN, COLL_EVERYTHING);
}

void Physics:: generateCamaro(){
    // Camaro
    glm::vec3 spawn = glm::vec3(0.0f, 0.0f, 0.0f);  // start position in world

    glm::vec3 car_pos = glm::vec3(0.0f, 1.0f, 0.0f) + spawn;
    glm::vec3 car_size = glm::vec3(1.0f, 0.6f, 3.0f);
    glm::vec3 car_rot = glm::vec3(0.0f, 0.0f, 0.0f);
    car = createRigidBody(BOX, car_pos, car_size, car_rot, GlobalVariables::car_mass, 1.75f, 0.2f, COLL_CHASSIS,
                                     COLL_EVERYTHING ^ COLL_CAR);
    car->setSleepingThresholds(0.0, 0.0);   // never stop simulating
    car->setDamping(GlobalVariables::cLinDamp * GlobalVariables::assist, GlobalVariables::cAngDamp * GlobalVariables::assist);

    btTransform frameA, frameB;

    glm::vec3 t1_pos = glm::vec3(-1.0f, 0.5f, -2.1f) + spawn;
    glm::vec3 t1_size = glm::vec3(0.4f, 0.35f, 0.35f);
    glm::vec3 t1_rot = glm::vec3(0.0f, 0.0f, glm::radians(-90.0f));
    t1 = createRigidBody(CYLINDER, t1_pos, t1_size, t1_rot, GlobalVariables::tyre_mass_1, GlobalVariables::tyre_friction, 0.0f, COLL_TYRE,
                                    COLL_EVERYTHING ^ COLL_CAR);
    t1->setSleepingThresholds(0.0, 0.0);    // never stop simulating
    t1->setDamping(GlobalVariables::tLinDamp * GlobalVariables::assist, GlobalVariables::tAngDamp * GlobalVariables::assist);
    frameA = btTransform::getIdentity();
    frameB = btTransform::getIdentity();
    frameA.getBasis().setEulerZYX(0, 0, 0);
    frameB.getBasis().setEulerZYX(0, 0, glm::radians(90.0f));
    frameA.setOrigin(btVector3(-1.0, -0.5, -2.1));
    frameB.setOrigin(btVector3(0.0, 0.0, 0.0));
    c1 = new btGeneric6DofSpringConstraint(*car, *t1, frameA, frameB, TRUE);
    c1->setLinearLowerLimit(btVector3(0, -GlobalVariables::lowLim, 0));
    c1->setLinearUpperLimit(btVector3(0, -GlobalVariables::upLim, 0));
    c1->setAngularLowerLimit(btVector3(1, -0.5, 0));
    c1->setAngularUpperLimit(btVector3(-1, 0.5, 0));
    c1->enableSpring(1, TRUE);
    c1->setStiffness(1, GlobalVariables::tyre_stiffness);
    c1->setDamping(1, GlobalVariables::tyre_damping);
    c1->setEquilibriumPoint();

    glm::vec3 t2_pos = glm::vec3(1.0f, 0.5f, -2.1f) + spawn;
    glm::vec3 t2_size = glm::vec3(0.4f, 0.35f, 0.35f);
    glm::vec3 t2_rot = glm::vec3(0.0f, 0.0f, glm::radians(90.0f));
    t2 = createRigidBody(CYLINDER, t2_pos, t2_size, t2_rot, GlobalVariables::tyre_mass_1, GlobalVariables::tyre_friction, 0.0f, COLL_TYRE,
                                    COLL_EVERYTHING ^ COLL_CAR);
    t2->setSleepingThresholds(0.0, 0.0);
    t2->setDamping(GlobalVariables::tLinDamp *GlobalVariables:: assist, GlobalVariables::tAngDamp * GlobalVariables::assist);
    frameA = btTransform::getIdentity();
    frameB = btTransform::getIdentity();
    frameA.getBasis().setEulerZYX(0, 0, 0);
    frameB.getBasis().setEulerZYX(0, 0, glm::radians(-90.0f));
    frameA.setOrigin(btVector3(1.0, -0.5, -2.1));
    frameB.setOrigin(btVector3(0.0, 0.0, 0.0));
    c2 = new btGeneric6DofSpringConstraint(*car, *t2, frameA, frameB, TRUE);
    c2->setLinearLowerLimit(btVector3(0, -GlobalVariables::lowLim, 0));
    c2->setLinearUpperLimit(btVector3(0, -GlobalVariables::upLim, 0));
    c2->setAngularLowerLimit(btVector3(1, -0.5, 0));
    c2->setAngularUpperLimit(btVector3(-1, 0.5, 0));
    c2->enableSpring(1, TRUE);
    c2->setStiffness(1, GlobalVariables::tyre_stiffness);
    c2->setDamping(1, GlobalVariables::tyre_damping);
    c2->setEquilibriumPoint();

    glm::vec3 t3_pos = glm::vec3(-1.0f, 0.5f, 1.6f) + spawn;
    glm::vec3 t3_size = glm::vec3(0.45f, 0.4f, 0.4f);
    glm::vec3 t3_rot = glm::vec3(0.0f, 0.0f, glm::radians(-90.0f));
    t3 = createRigidBody(CYLINDER, t3_pos, t3_size, t3_rot, GlobalVariables::tyre_mass_2, GlobalVariables::tyre_friction, 0.0f, COLL_TYRE,
                                    COLL_EVERYTHING ^ COLL_CAR);
    t3->setSleepingThresholds(0.0, 0.0);
    t3->setDamping(GlobalVariables::tLinDamp * GlobalVariables::assist, GlobalVariables::tAngDamp * GlobalVariables::assist);
    frameA = btTransform::getIdentity();
    frameB = btTransform::getIdentity();
    frameA.getBasis().setEulerZYX(0, 0, 0);
    frameB.getBasis().setEulerZYX(0, 0, glm::radians(90.0f));
    frameA.setOrigin(btVector3(-1.0, -0.5, 1.6));
    frameB.setOrigin(btVector3(0.0, 0.0, 0.0));
    c3 = new btGeneric6DofSpringConstraint(*car, *t3, frameA, frameB, TRUE);
    c3->setLinearLowerLimit(btVector3(0, -GlobalVariables::lowLim, 0));
    c3->setLinearUpperLimit(btVector3(0, -GlobalVariables::upLim, 0));
    c3->setAngularLowerLimit(btVector3(1, 0, 0));
    c3->setAngularUpperLimit(btVector3(-1, 0, 0));
    c3->enableSpring(1, TRUE);
    c3->setStiffness(1, GlobalVariables::tyre_stiffness);
    c3->setDamping(1, GlobalVariables::tyre_damping);
    c3->setEquilibriumPoint();

    glm::vec3 t4_pos = glm::vec3(1.0f, 0.5f, 1.6f) + spawn;
    glm::vec3 t4_size = glm::vec3(0.45f, 0.4f, 0.4f);
    glm::vec3 t4_rot = glm::vec3(0.0f, 0.0f, glm::radians(90.0f));
    t4 = createRigidBody(CYLINDER, t4_pos, t4_size, t4_rot, GlobalVariables::tyre_mass_2, GlobalVariables::tyre_friction, 0.0f, COLL_TYRE,
                                    COLL_EVERYTHING ^ COLL_CAR);
    t4->setSleepingThresholds(0.0, 0.0);
    t4->setDamping(GlobalVariables::tLinDamp * GlobalVariables::assist, GlobalVariables::tAngDamp * GlobalVariables::assist);
    frameA = btTransform::getIdentity();
    frameB = btTransform::getIdentity();
    frameA.getBasis().setEulerZYX(0, 0, 0);
    frameB.getBasis().setEulerZYX(0, 0, glm::radians(-90.0f));
    frameA.setOrigin(btVector3(1.0, -0.5, 1.6));
    frameB.setOrigin(btVector3(0.0, 0.0, 0.0));
    c4 = new btGeneric6DofSpringConstraint(*car, *t4, frameA, frameB, TRUE);
    c4->setLinearLowerLimit(btVector3(0, -GlobalVariables::lowLim, 0));
    c4->setLinearUpperLimit(btVector3(0, -GlobalVariables::upLim, 0));
    c4->setAngularLowerLimit(btVector3(1, 0, 0));
    c4->setAngularUpperLimit(btVector3(-1, 0, 0));
    c4->enableSpring(1, TRUE);
    c4->setStiffness(1, GlobalVariables::tyre_stiffness);
    c4->setDamping(1, GlobalVariables::tyre_damping);
    c4->setEquilibriumPoint();
}

void Physics:: addConstraints(){
    dynamicsWorld->addConstraint(c1);
    dynamicsWorld->addConstraint(c2);
    dynamicsWorld->addConstraint(c3);
    dynamicsWorld->addConstraint(c4);
}

void Physics:: updateMovements(){
    btMatrix3x3 rot = car->getWorldTransform().getBasis();
    short braking = 1;
    accelerationMovement(rot, braking);
    brakingMovement(rot, braking);
    handbrakeMovement(rot, braking);
    resetMovement(rot);
    jumpMovement();
}

void Physics:: accelerationMovement(btMatrix3x3 &rot, short &braking){
    float linearVelocity = car->getLinearVelocity().length();
    if (GlobalVariables::acceleration < 0 && linearVelocity > GlobalVariables::maxVelocity / 10) {
        braking = 0;
    } else {
        if (linearVelocity < GlobalVariables::maxVelocity / (1 + 9 * (GlobalVariables::acceleration < 0))) {
            float torque = -GlobalVariables::maxAcceleration * GlobalVariables::acceleration * (1 - (abs(GlobalVariables::steering) * (linearVelocity > 10)) / 2);
            t1->applyTorque(rot * btVector3(torque, 0, 0));
            t2->applyTorque(rot * btVector3(torque, 0, 0));
            if (!GlobalVariables::handbrake) {
                t3->applyTorque(rot * btVector3(torque, 0, 0));
                t4->applyTorque(rot * btVector3(torque, 0, 0));
            }
        }
    }
}
void Physics:: brakingMovement(btMatrix3x3 &rot, short &braking){
    c1->setAngularLowerLimit(btVector3(braking, GlobalVariables::tyre_steering_angle * GlobalVariables::steering, 0));
    c1->setAngularUpperLimit(btVector3(-braking, GlobalVariables::tyre_steering_angle * GlobalVariables::steering, 0));
    c2->setAngularLowerLimit(btVector3(braking, GlobalVariables::tyre_steering_angle * GlobalVariables::steering, 0));
    c2->setAngularUpperLimit(btVector3(-braking, GlobalVariables::tyre_steering_angle * GlobalVariables::steering, 0));

}
void Physics:: handbrakeMovement(btMatrix3x3 &rot, short &braking){
    if (GlobalVariables::handbrake) {
        c3->setAngularLowerLimit(btVector3(0, 0, 0));
        c3->setAngularUpperLimit(btVector3(0, 0, 0));
        c4->setAngularLowerLimit(btVector3(0, 0, 0));
        c4->setAngularUpperLimit(btVector3(0, 0, 0));
    } else {
        c3->setAngularLowerLimit(btVector3(braking, 0, 0));
        c3->setAngularUpperLimit(btVector3(-braking, 0, 0));
        c4->setAngularLowerLimit(btVector3(braking, 0, 0));
        c4->setAngularUpperLimit(btVector3(-braking, 0, 0));
    }
}
void Physics:: resetMovement(btMatrix3x3 &rot){
    if (GlobalVariables::getUp) {
        car->applyTorqueImpulse(rot * btVector3(0, 0, 12000));
    }
}
void Physics:: jumpMovement(){
    if (GlobalVariables::jump) {
        car->applyCentralImpulse(btVector3(0, 10000, 0));
    }
}
