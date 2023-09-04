////
//// Created by Elena on 09.7.2023.
////
//
#ifndef KOMPJUTERSKA_GRAFIKA_PROJEKT_GAME_H
#define KOMPJUTERSKA_GRAFIKA_PROJEKT_GAME_H
#pragma once

#include "Car.h"
#include "Terrain.h"
#include "Physics.h"
#include "GLFW/glfw3.h"
#include "GlobalVariables.h"

class Game {
public:
    Car cars[3];
    Terrain terrain;
    Physics simulation;
    bool gameStarted;
    float rotationAngle;
    unsigned int cubeMapTexture;
    glm::mat4 projection;
    glm::mat4 view;
    int chosenCarIndex;

    Game(const string& carShaderVertexPath,
         const string& carShaderFragmentPath,
         const string& carOneModelPath,
         const string& carTwoModelPath,
         const string& carThreeModelPath,
         const string& tyre1ModelPath,
         const string& tyre2ModelPath,
         const string& terrainShaderVertexPath,
         const string& terrainShaderFragmentPath,
         const string& terrainModel1Path,
         const string& terrainModel2Path,
         const string& terrainModel3Path
         );

    void initialize();
    void preGame(glm::mat4 &objModelMatrix, glm::mat3 &objNormalMatrix, int &num_cobjs,
                 Model *objectModel, GLfloat *matrix, btTransform transform);
    void startGame(glm::mat4 &objModelMatrix, glm::mat3 &objNormalMatrix, int &num_cobjs,
                   Model *objectModel, GLfloat *matrix, btTransform transform);
    void updateCameraPosition() const;
    void transform();
    void carUniFrom(glm::mat4& objModelMatrix,glm::mat3& objNormalMatrix) const;
private:
};
#endif //KOMPJUTERSKA_GRAFIKA_PROJEKT_GAME_H
