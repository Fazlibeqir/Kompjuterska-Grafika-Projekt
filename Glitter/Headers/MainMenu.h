//
// Created by Elena on 09.7.2023.
//

#ifndef KOMPJUTERSKA_GRAFIKA_PROJEKT_MAINMENU_H
#define KOMPJUTERSKA_GRAFIKA_PROJEKT_MAINMENU_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

class MainMenu {
public:
    MainMenu();
    void renderMainMenu();
    void initializeImGui(GLFWwindow* window);
    static void cleanImGui();
    void renderImGui();
//
//    static void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
//    static void cursorPosCallback(GLFWwindow* window, double xpos, double ypos);

private:
    GLFWwindow* window{};
    void startGame();
    void quitGame();
    void showSettings();
};


#endif //KOMPJUTERSKA_GRAFIKA_PROJEKT_MAINMENU_H
