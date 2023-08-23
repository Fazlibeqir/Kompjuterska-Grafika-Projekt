//
// Created by Elena on 19.8.2023.
//

#ifndef KOMPJUTERSKA_GRAFIKA_PROJEKT_GLOBALVARIABLES_H
#define KOMPJUTERSKA_GRAFIKA_PROJEKT_GLOBALVARIABLES_H

#include "camera.hpp"
#include "BulletDynamics/Dynamics/btRigidBody.h"
#include "BulletDynamics/ConstraintSolver/btGeneric6DofSpringConstraint.h"

class GlobalVariables{
public:
    static float deltaTime;
    static float lastFrame;
    static const unsigned int scrWidth = 960;
    static const unsigned int scrHeight = 540;
    // Camera controls
    static Camera camera;
    static bool cameraFollow;
    static glm::vec3 cameraFollowPos;
//    static auto cameraFollowP;
//    static auto cameraFollowY;
    static float lastX;
    static float lastY;
    static bool firstMouse;
    static bool rotating;
    static float cameraRadius;
    static bool switched;

// Car controls
    static short acceleration;
    static float steering;
    static bool handbrake;
    static float maxAcceleration;     // engine torque
    static float maxVelocity;          // engine speed limit
    static bool getUp, gotUp;
    static bool jump, jumped;
    static float basePitch, baseYaw;

// Car properties
    static float car_mass;
    static float tyre_mass_1;          // front wheels weight
    static float tyre_mass_2;          // rear wheels weight
    static float tyre_friction;
    static float tyre_stiffness;   // suspension travel
    static float tyre_damping;    // suspension oscillation
    static float tyre_steering_angle;
    static float assist;
    static float lowLim;
    static float upLim;
    static const float cLinDamp;
    static const float cAngDamp;
    static const float tLinDamp;
    static const float tAngDamp;

    static const unsigned int grid_width = 10;
    static const unsigned int grid_height = 8;
    static const unsigned int tiles = grid_width * grid_height;
    static const unsigned int track[grid_height][grid_width];
    static const float plane_edge;
    static const unsigned int walls = 4;

    static GLfloat maxSecPerFrame;
    static glm::vec3 lightPos;
};


#endif //KOMPJUTERSKA_GRAFIKA_PROJEKT_GLOBALVARIABLES_H
