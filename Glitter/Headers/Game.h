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
         const std::string& skyVertPath, const std::string& skyFragPath,
         const std::string& heightVertPath,const std::string&heightFragPath,
         const std::string& modelPath);

    Shader ourShader;
    Model car;
    FrameBuffer frameBuffer;
    Camera camera;

    void initShaders();
    void initialStart();
    void setUniforms(Shader& shader);
    void start(GLFWwindow* window);
    static void quit(GLFWwindow* window);
    void settings();
    bool gameStarted = false;
    void setRotationAngle();
private:
    int idx=0;
    static glm::vec3 lightDirection();
    float ambientS;
    float diffuseS;
    float specularS;
    float scale;
};


#endif //KOMPJUTERSKA_GRAFIKA_PROJEKT_GAME_H
