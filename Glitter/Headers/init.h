////
//// Created by Elena on 08.7.2023.
////
//
//#ifndef KOMPJUTERSKA_GRAFIKA_PROJEKT_INIT_H
//#define KOMPJUTERSKA_GRAFIKA_PROJEKT_INIT_H
//
//#pragma once
//#include "Shader.hpp"
//#include "camera.hpp"
//#include "model.hpp"
//#include "Physics.h"
//
//
//#include <glad/glad.h>
//#include <GLFW/glfw3.h>
//#include <glm/glm.hpp>
//#include <glm/gtc/matrix_transform.hpp>
//#include <glm/gtc/type_ptr.hpp>
//
//
//
//#include <iostream>
//#include <cstdio>
//#include <cstdlib>
//#include <fstream>
//#include <sstream>
//#include <streambuf>
//#include <string>
//
//const unsigned int scrWidth = 960;
//const unsigned int scrHeight = 540;
//const char *appName = "Velocity Xtreme";
//
//
//
//// General callback functions
//void mouseCallback(GLFWwindow *window, double xposd, double yposd);
//
//void mouseButtonCallback(GLFWwindow *window, int button, int action, int mods);
//
//void scrollCallback(GLFWwindow *window, double d, double y);
//
//void processInput(GLFWwindow *window
//                  //, bool gameHasStarted
//                  );
//
//unsigned int loadCubeMap();
//
//// Camera controls
//Camera camera(glm::vec3(0.0f, 2.5f, 8.0f), GL_FALSE);
//bool cameraFollow = TRUE;
//glm::vec3 cameraFollowPos(0.0f);
//auto cameraFollowP = 0.0f;
//auto cameraFollowY = 0.0f;
//float lastX = static_cast<float>(scrWidth) / 2.0f;
//float lastY = static_cast<float>(scrHeight) / 2.0f;
//bool firstMouse = TRUE;
//bool rotating = FALSE;
//float cameraRadius = 8.0f;
//bool switched = FALSE;
//
//// Car controls
//short acceleration = 0;
//float steering = 0.0f;
//bool handbrake = FALSE;
//float maxAcceleration = 800.0f;     // engine torque
//float maxVelocity = 50.0f;          // engine speed limit
//bool getUp = FALSE, gotUp = FALSE;
//bool jump = FALSE, jumped = FALSE;
//float basePitch = 0.0f, baseYaw = 0.0f;
//
//// Car properties
//float car_mass = 1250.0f;
//float tyre_mass_1 = 15.0f;          // front wheels weight
//float tyre_mass_2 = 20.0f;          // rear wheels weight
//float tyre_friction = 2.25f;
//float tyre_stiffness = 120000.0f;   // suspension travel
//float tyre_damping = 0.0000200f;    // suspension oscillation
//float tyre_steering_angle = 0.5f;
//float assist = 0.5f;
//float lowLim = 0.0f;
//float upLim = 0.1f;
//const float cLinDamp = 0.02f;
//const float cAngDamp = 0.4f;
//const float tLinDamp = 0.01f;
//const float tAngDamp = 0.2f;
//
//// Car components
//btRigidBody *car, *t1, *t2, *t3, *t4;
//btGeneric6DofSpringConstraint *c1, *c2, *c3, *c4;
//
//// Delta time
//float deltaTime = 0.0f;
//float lastFrame = 0.0f;
//
//
//
//#endif //KOMPJUTERSKA_GRAFIKA_PROJEKT_INIT_H
