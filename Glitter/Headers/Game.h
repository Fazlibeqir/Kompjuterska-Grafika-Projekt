//
// Created by Elena on 09.7.2023.
//

#ifndef KOMPJUTERSKA_GRAFIKA_PROJEKT_GAME_H
#define KOMPJUTERSKA_GRAFIKA_PROJEKT_GAME_H
#pragma once

#include "shader.hpp"
#include "Skybox.hpp"
#include "Model.hpp"
#include "init.h"
#include "GLFW/glfw3.h"
#include "camera.hpp"
#include "CarPhysics.hpp"
class Game {
public:
    Game(const std::string& shaderVertPath, const std::string& shaderFragPath,
         const std::string& skyVertPath, const std::string& skyFragPath,
         const std::string& modelPath,const std::string& mapModelPath);

    Shader ourShader;
    Shader skyboxShader;
    Model car;
    Model map;
    Camera camera;

    void initShaders();
    void initialStart();
    void start(GLFWwindow* window);
    static void quit(GLFWwindow* window);
    void settings();
    void processInput(GLFWwindow* window);
    bool shouldReturnToMenu() const { return returnToMenuClicked; }
    bool shouldStartTheGame() const{return startWithEnter;}
private:


    void updateDeltaTime();
    void setUniforms();
    void initSkybox();
    void initTextures();
    void renderSkybox();
    static GLuint loadTexture(GLchar* path);
    static GLuint loadCubemap(vector<const GLchar*> faces);
    static glm::vec3 lightDirection();

    GLuint VBO{}, VAO{}, EBO{};
    GLuint skyboxVAO{}, skyboxVBO{};
    GLuint texture;
    GLuint cubemapTexture;
    glm::mat4 view;
    glm::mat4 model;
    glm::mat4 projection;
    bool startWithEnter=false;
    bool returnToMenuClicked = false;
    int idx=0;
    float cameraSpeed;
    float rotationAngle;
    float deltaTime;
public:
    float getDeltaTime() const;

private:
    float lastFrame;
    float ambientS;
    float diffuseS;
    float specularS;
    float scale;

    // Add CarPhysics instance
    CarPhysics carPhysics;

    // Add private functions for car rendering
    void renderCar();

    // Add private functions for racetrack rendering
    void renderRacetrack(Shader& shader);

};


#endif //KOMPJUTERSKA_GRAFIKA_PROJEKT_GAME_H
