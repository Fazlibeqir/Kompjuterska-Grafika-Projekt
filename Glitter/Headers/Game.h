////
//// Created by Elena on 09.7.2023.
////
//
//#ifndef KOMPJUTERSKA_GRAFIKA_PROJEKT_GAME_H
//#define KOMPJUTERSKA_GRAFIKA_PROJEKT_GAME_H
//#pragma once
//
//#include "shader.hpp"
//#include "Model.hpp"
//#include "init.h"
//#include "GLFW/glfw3.h"
//#include "camera.hpp"
//#include "Physics.h"
//#include "FrameBuffer.hpp"
//#include <imgui.h>
//#include <imgui_impl_glfw.h>
//#include <imgui_impl_opengl3.h>
//
//class Game {
//public:
//    Game(const std::string& shaderVertPath, const std::string& shaderFragPath,
//         const std::string& skyVertPath, const std::string& skyFragPath,
//         const std::string& heightVertPath,const std::string&heightFragPath,
//         const std::string modelPaths);
//
//    Shader ourShader;
//    Model cars;
//    FrameBuffer frameBuffer;
//    Camera camera;
//
//    void initShaders();
//    void initialStart();
//    void setUniforms(Shader& shader);
//    void start(GLFWwindow* window);
//    static void quit(GLFWwindow* window);
//    void settings();
//    bool gameStarted = false;
//    void setRotationAngle();
//    void RenderScore();
//    int chosenCarIndex;
//    // in order to keep track which car is being chosen
//    // it is better to keep its index,
//    // instead of copying the data from one Model to another.
//private:
//    int idx=0;
//    static glm::vec3 lightDirection();
//    float ambientS;
//    float diffuseS;
//    float specularS;
//    float scale;
//    int score;
//
//};
//
//
//#endif //KOMPJUTERSKA_GRAFIKA_PROJEKT_GAME_H
