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

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow *window);
const unsigned int SCR_WIDTH = 1200;
const unsigned int SCR_HEIGHT = 700;

extern Camera camera;
extern float lastX;
extern float lastY;
extern bool firstMouse;

GLFWwindow* initializeWindow();
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xposIn, double yposIn);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow *window);

#endif //KOMPJUTERSKA_GRAFIKA_PROJEKT_INIT_H
