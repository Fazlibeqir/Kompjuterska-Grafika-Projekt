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

    void initialStart();
    void start();
    static void quit(GLFWwindow* window);
    void settings();
    static void processInput(GLFWwindow* window);
    static GLuint loadTexture(GLchar* path);
    static GLuint loadCubemap(vector<const GLchar*> faces);


private:
    GLuint VBO{}, VAO{}, EBO{};
    GLuint skyboxVAO{}, skyboxVBO{};
    GLuint cubemapTexture;
    int idx=0;
    static glm::vec3 lightDirection();
    float rotationAngle;
    float deltaTime;
    float lastFrame;
    float ambientS = 0.5;
    float diffuseS = 1.5;
    float specularS = 0.3;
    char* planeText[];


};


#endif //KOMPJUTERSKA_GRAFIKA_PROJEKT_GAME_H
