//
// Created by Elena on 09.7.2023.
//

#include "MainMenu.h"


MainMenu::MainMenu(GLFWwindow* inWindow)
: window(inWindow),volume(0.5f){
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    ImGui::StyleColorsDark();
}

void MainMenu::renderMainMenu(){
    ImGui::SetNextWindowPos(ImVec2(50, 50));  // Set the desired position
    ImGui::SetNextWindowSize(ImVec2(220, 220)); // Set the desired size
    ImGui::Begin("Main Menu",nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove);
    ImGui::Separator();
    // Set the button size and style
    ImVec2 buttonSize(200, 40);
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 10));
    if (ImGui::Button("Start Game",buttonSize)|| glfwGetKey(window, GLFW_KEY_ENTER) == GLFW_PRESS)
    {
        // Handle start game action
        gameStarted = true;
        hide();

    }else{
        gameStarted = false;
    }

    if (ImGui::Button("Settings",buttonSize))
    {
        // Handle settings action
        showSettings();
    }

    if (ImGui::Button("Quit",buttonSize))
    {
        // Handle quit action
        quitGame();
    }

    ImGui::Text("Right or left to choose a car");
    ImGui::PopStyleVar();
    ImGui::End();
}
void MainMenu:: initializeImGui() const
{
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330 core");
}

void MainMenu:: cleanImGui(){
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}


void MainMenu:: renderImGui(){
    if(isMenuVisible()) {
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        renderMainMenu();
        if (showSettingsWindow) {
            showSettings();
        }

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    }
    else{
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        //game.RenderScore();

        ImGui::Render();
    }
}

void MainMenu::quitGame() const {
    audio.~Audio();
    cleanImGui();
    glfwDestroyWindow(window);
    glfwTerminate();
}

void MainMenu::showSettings() {
    showSettingsWindow = true;
    ImGui::Begin("Settings");
    ImGui::SliderFloat("Volume", &volume, 0.0f, 1.0f);
    if (ImGui::Button("Apply")) {
        // Apply volume changes to the audio
        audio.setDefaultVolume(volume);
    }
    ImGui::End();
}
