//
// Created by Elena on 09.7.2023.
//

#ifndef KOMPJUTERSKA_GRAFIKA_PROJEKT_MAINMENU_H
#define KOMPJUTERSKA_GRAFIKA_PROJEKT_MAINMENU_H
#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>
#include "Game.h"
#include <string> // Add this line

class MainMenu {
public:
    MainMenu(GLFWwindow* inWindow, const std::string& shaderVertPath, const std::string& shaderFragPath, const std::string& modelPath);
    void renderMainMenu();
    void initializeImGui();
    void cleanImGui();
    void renderImGui();
    GLFWwindow* window;
    Game game;

//
//    static void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
//    static void cursorPosCallback(GLFWwindow* window, double xpos, double ypos);

private:
    void startGame();
    void quitGame();
    void showSettings();
};


#endif //KOMPJUTERSKA_GRAFIKA_PROJEKT_MAINMENU_H
