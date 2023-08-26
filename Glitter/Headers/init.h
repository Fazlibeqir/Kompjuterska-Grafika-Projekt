////
//// Created by Elena on 08.7.2023.
////
//
#ifndef KOMPJUTERSKA_GRAFIKA_PROJEKT_INIT_H
#define KOMPJUTERSKA_GRAFIKA_PROJEKT_INIT_H
#pragma once

#include <iostream>
#include "glad/glad.h"
#include "glm/vec3.hpp"
#include "GLFW/glfw3.h"
#include "camera.hpp"
#include "Audio.hpp"
#include <map>
#include <string>


using namespace std;
class Init{
public:
    // Delta time
    Init();
    static void mouseCallback(GLFWwindow *window, double xposd, double yposd);
    static void mouseButtonCallback(GLFWwindow *window, int button, int action, int mods);
    static void scrollCallback(GLFWwindow *window, double d, double y);
    void processInput(GLFWwindow *window);
    void processInputForGame(GLFWwindow *window);
    static GLFWwindow * initializeWindow();
    static void updateDeltaTime();
    static map<const string, string> initializeShadersAndModelsPaths();

    static void processInputForPreGame(GLFWwindow *window, int &chosenCar);

private:
};

#endif //KOMPJUTERSKA_GRAFIKA_PROJEKT_INIT_H
