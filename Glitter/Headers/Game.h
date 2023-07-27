//
// Created by Elena on 09.7.2023.
//

#ifndef KOMPJUTERSKA_GRAFIKA_PROJEKT_GAME_H
#define KOMPJUTERSKA_GRAFIKA_PROJEKT_GAME_H
#pragma once

#include "shader.hpp"
#include "Model.hpp"
#include "init.h"
#include "GLFW/glfw3.h"
#include "camera.hpp"
#include "Physics.h"
#include "FrameBuffer.hpp"

class Game {
public:
    Game(const std::string& shaderVertPath, const std::string& shaderFragPath,
         const std::string& skyVertPath, const std::string& skyFragPath, const std::string& modelPath);

    Shader ourShader;
    Model car;
    FrameBuffer frameBuffer;
    Camera camera;

    void initShaders();
    void initialStart();
    void updateDeltaTime();
    void setUniforms();
    void start(GLFWwindow* window);
    static void quit(GLFWwindow* window);
    void settings();
    void processInput(GLFWwindow* window);
    bool shouldReturnToMenu() const { return returnToMenuClicked; }
    bool gameStarted = false;
    void setRotationAngle();
private:

    float carAcc;
    float carTurn;
    bool returnToMenuClicked = false;
    int idx=0;
    static glm::vec3 lightDirection();
    float rotationAngle;
    float deltaTime;
    float lastFrame;
    float ambientS;
    float diffuseS;
    float specularS;
    float scale;

};


#endif //KOMPJUTERSKA_GRAFIKA_PROJEKT_GAME_H
