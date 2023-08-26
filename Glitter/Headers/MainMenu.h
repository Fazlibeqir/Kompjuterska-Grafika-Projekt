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
    void renderScoreWindow();
    void initializeImGui() const;
    static void cleanImGui();
    void renderImGui();
    bool hasCrossedFinishLine(const glm::vec3& currentPlayerPosition);
    void setPlayerPosition(const glm::vec3& position);
    void updateRaceStatus();
    void hide() { showMenu = false; }
    void show() { showMenu = true; }
    bool isMenuVisible() const {
        return showMenu;
    }

    bool disableInputForGame;
    bool gameStarted;
    GLFWwindow* window;
private:
    float finishedRaceTime = 0.0f;
    float countdownTimer = 10.0f;
    int crossingCount=0;
    Audio audio;
    bool showSettingsWindow = false;
    bool raceFinished = false;
    glm::vec3 playerPosition; // Store the player's position
    float volume;
    bool showMenu= true;
    void quitGame() const;
    void showSettings();
    void toggleFullScreen();
    void currentlyPlaying();
};


#endif //KOMPJUTERSKA_GRAFIKA_PROJEKT_MAINMENU_H
