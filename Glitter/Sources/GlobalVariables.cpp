//
// Created by Elena on 19.8.2023.
//
#include "GlobalVariables.h"

float GlobalVariables:: deltaTime = 0.0f;
float GlobalVariables:: lastFrame = 0.0f;

Camera GlobalVariables::camera(glm::vec3(0.0f, 2.5f, 8.0f), GL_FALSE);
bool GlobalVariables::cameraFollow = TRUE;
glm::vec3 GlobalVariables:: cameraFollowPos(0.0f);
//auto  cameraFollowY = 0.0f;
float GlobalVariables:: lastX = 480.0f;
float GlobalVariables:: lastY = 270.0f;
bool GlobalVariables:: firstMouse = TRUE;
bool GlobalVariables:: rotating = FALSE;
float GlobalVariables:: cameraRadius = 8.0f;
bool GlobalVariables:: switched = FALSE;

// Car controls
short GlobalVariables:: acceleration = 0;
float GlobalVariables:: steering = 0.0f;
bool GlobalVariables:: handbrake = FALSE;
float GlobalVariables:: maxAcceleration = 800.0f;     // engine torque
float GlobalVariables:: maxVelocity = 50.0f;          // engine speed limit
bool GlobalVariables:: getUp = FALSE;
bool GlobalVariables::gotUp = FALSE;
bool GlobalVariables:: jump = FALSE;
bool GlobalVariables:: jumped = FALSE;
float GlobalVariables:: basePitch = 0.0f;
float GlobalVariables:: baseYaw = 0.0f;

// Car properties
float GlobalVariables:: car_mass = 1250.0f;
float GlobalVariables:: tyre_mass_1 = 15.0f;          // front wheels weight
float GlobalVariables:: tyre_mass_2 = 20.0f;          // rear wheels weight
float GlobalVariables:: tyre_friction = 2.25f;
float GlobalVariables:: tyre_stiffness = 120000.0f;   // suspension travel
float GlobalVariables:: tyre_damping = 0.0000200f;    // suspension oscillation
float GlobalVariables:: tyre_steering_angle = 0.5f;
float GlobalVariables:: assist = 0.5f;
float GlobalVariables:: lowLim = 0.0f;
float GlobalVariables:: upLim = 0.1f;
const float GlobalVariables:: cLinDamp = 0.02f;
const float GlobalVariables:: cAngDamp = 0.4f;
const float GlobalVariables:: tLinDamp = 0.01f;
const float GlobalVariables:: tAngDamp = 0.2f;
const unsigned int GlobalVariables::track[grid_height][grid_width] = {
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
};

const float GlobalVariables:: plane_edge = 20.0f;

float GlobalVariables:: maxSecPerFrame = 1.0f / 50.0f;
glm::vec3 GlobalVariables:: lightPos(0.0,2.0,-1.0);
