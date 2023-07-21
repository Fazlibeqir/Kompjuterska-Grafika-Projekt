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
class Game {
public:
    Game(const std::string& shaderVertPath, const std::string& shaderFragPath,
         const std::string& skyVertPath, const std::string& skyFragPath, const std::string& modelPath);

    Shader ourShader;
    Shader skyboxShader;
    Model car;
    Camera camera;

    void initSkybox();
    void initTextures();
    void initShaders();
    void initialStart();
    void updateDeltaTime();
    void setUniforms();
    void renderSkybox();
    void start();
    static void quit(GLFWwindow* window);
    void settings();
    void processInput(GLFWwindow* window);
    static GLuint loadTexture(GLchar* path);
    static GLuint loadCubemap(vector<const GLchar*> faces);
    bool shouldReturnToMenu() const { return returnToMenuClicked; }


private:
    GLuint VBO{}, VAO{}, EBO{};
    GLuint skyboxVAO{}, skyboxVBO{};
    GLuint texture;
    GLuint cubemapTexture;
    glm::mat4 view;
    glm::mat4 model;
    glm::mat4 projection;
    bool returnToMenuClicked = false;
    int idx=0;
    static glm::vec3 lightDirection();
    float rotationAngle;
    float deltaTime;
    float lastFrame;
    float ambientS = 0.5;
    float diffuseS = 1.5;
    float specularS = 0.3;
    float scale;

};


#endif //KOMPJUTERSKA_GRAFIKA_PROJEKT_GAME_H
