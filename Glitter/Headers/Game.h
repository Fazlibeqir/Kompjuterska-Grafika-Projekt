////
//// Created by Elena on 09.7.2023.
////
//
#ifndef KOMPJUTERSKA_GRAFIKA_PROJEKT_GAME_H
#define KOMPJUTERSKA_GRAFIKA_PROJEKT_GAME_H
#pragma once

#include "../Sources/Car.h"
#include "../Sources/Terrain.h"
#include "Skybox.hpp"
#include "Physics.h"
#include "GLFW/glfw3.h"
#include "GlobalVariables.h"

class Game {
public:
    Car carForGame;
    Terrain terrain;
    Skybox skybox;
    Physics simulation;
    bool gameStarted;
    float rotationAngle;
    unsigned int skyboxVAO, skyboxVBO;
    unsigned int cubeMapTexture;
    glm::mat4 projection;
    glm::mat4 view;

    Game(const string& carShaderVertexPath,
         const string& carShaderFragmentPath,
         const string& carModelPath,
         const string& tyre1ModelPath,
         const string& tyre2ModelPath,
         const string& terrainShaderVertexPath,
         const string& terrainShaderFragmentPath,
         const string& terrainModel1Path,
         const string& terrainModel2Path,
         const string& skyboxShaderVertexPath,
         const string& skyboxShaderFragmentPath);

    void initialize();
    void preGame();
    void startGame(GLFWwindow *window);
    void updateCameraPosition();
    void transform();
    void setSkybox();

private:

};


#endif //KOMPJUTERSKA_GRAFIKA_PROJEKT_GAME_H
