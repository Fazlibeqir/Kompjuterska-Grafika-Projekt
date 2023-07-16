//
// Created by Elena on 09.7.2023.
//

#ifndef KOMPJUTERSKA_GRAFIKA_PROJEKT_GAME_H
#define KOMPJUTERSKA_GRAFIKA_PROJEKT_GAME_H
#pragma once

#include "shader.hpp"
#include "Model.hpp"
#include "GLFW/glfw3.h"
#include "camera.hpp"

class Game {
public:
    Game(const std::string& shaderVertPath, const std::string& shaderFragPath, const std::string& modelPath);

    Shader ourShader;
    Model car;
    Camera camera;

    void initialStart();
    void start();
    void quit(GLFWwindow* window);
    void settings();
    void processInput(GLFWwindow* window);

private:
    float rotationAngle;
    float deltaTime;
    float lastFrame;
};


#endif //KOMPJUTERSKA_GRAFIKA_PROJEKT_GAME_H
