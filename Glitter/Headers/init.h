//
// Created by Elena on 08.7.2023.
//

#ifndef KOMPJUTERSKA_GRAFIKA_PROJEKT_INIT_H
#define KOMPJUTERSKA_GRAFIKA_PROJEKT_INIT_H

#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "camera.hpp"
#include <iostream>

const unsigned int SCR_WIDTH = 1200;
const unsigned int SCR_HEIGHT = 700;
extern float deltaTime;
extern float lastFrame;
extern bool returnToMenuClicked;
extern float rotationAngle;
extern glm::vec3 carPosition;
extern glm::vec3 carFront;
extern float carSpeed;
extern float carRotationSpeed;

extern Camera camera;
extern float lastX;
extern float lastY;
extern bool firstMouse;

GLFWwindow* initializeWindow();
void updateDeltaTime();
void framebuffer_size_callback(GLFWwindow *window, int width, int height);
//void mouse_callback(GLFWwindow *window, double xpos, double ypos);
void scroll_callback(GLFWwindow *window, double xoffset, double yoffset);
void processInput(GLFWwindow *window, int *chosenCarIndex, bool gameHasStarted);




#endif //KOMPJUTERSKA_GRAFIKA_PROJEKT_INIT_H
