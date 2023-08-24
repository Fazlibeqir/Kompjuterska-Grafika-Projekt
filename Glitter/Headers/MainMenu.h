//
// Created by Elena on 09.7.2023.
//

#ifndef KOMPJUTERSKA_GRAFIKA_PROJEKT_MAINMENU_H
#define KOMPJUTERSKA_GRAFIKA_PROJEKT_MAINMENU_H



#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include "Game.h"
#include "Audio.hpp"
#include <string>

class MainMenu {
public:
    MainMenu(GLFWwindow* inWindow);
    void renderMainMenu();
    void renderMainMenuLogic(const ImVec2& buttonSize);
    void initializeImGui() const;
    static void cleanImGui();
    void renderImGui();
    void hide() { showMenu = false; }
    void show() { showMenu = true; }
    bool isMenuVisible() const {
        return showMenu;
    }

    bool gameStarted;
    GLFWwindow* window;
private:
    Audio audio;
    bool showSettingsWindow = false;
    float volume;
    bool showMenu= true;
    void quitGame() const;
    void showSettings();
};


#endif //KOMPJUTERSKA_GRAFIKA_PROJEKT_MAINMENU_H
