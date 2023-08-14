//
// Created by Elena on 09.7.2023.
//

#ifndef KOMPJUTERSKA_GRAFIKA_PROJEKT_MAINMENU_H
#define KOMPJUTERSKA_GRAFIKA_PROJEKT_MAINMENU_H



#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include "Game.h"
#include <string>

class MainMenu {
public:
    MainMenu(GLFWwindow* inWindow, const std::string& shaderVertPath, const std::string& shaderFragPath,
             const std::string& skyVertPath, const std::string& skyFragPath,
             const std::string& heightVertPath, const std::string& heightFragPath,
             const std::string modelPaths[]);
    void renderMainMenu();
    void initializeImGui() const;
    static void cleanImGui();
    void renderImGui();
    void hide() { showMenu = false; }
    void show() { showMenu = true; }
    bool isMenuVisible() const {
        return showMenu;
    }

    GLFWwindow* window;
    Game game;
//
//    static void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
//    static void cursorPosCallback(GLFWwindow* window, double xpos, double ypos);

private:
    bool showMenu= true;
    void quitGame() const;
    void showSettings();
};


#endif //KOMPJUTERSKA_GRAFIKA_PROJEKT_MAINMENU_H
