//
// Created by Elena on 09.7.2023.
//

#include "MainMenu.h"

MainMenu::MainMenu(GLFWwindow* inWindow, const std::string& shaderVertPath, const std::string& shaderFragPath,const std::string& skyVertPath, const std::string& skyFragPath, const std::string& modelPath)
: window(inWindow), game(shaderVertPath, shaderFragPath,skyVertPath, skyFragPath,modelPath)
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    ImGui::StyleColorsDark();
}

void MainMenu::renderMainMenu(){
    ImGui::Begin("Main Menu");
    if (ImGui::Button("Start Game"))
    {
        // Handle start game action
        startGameClicked = true;
        startGame();
    }else{
        startGameClicked = false;
    }

    if (ImGui::Button("Settings"))
    {
        // Handle settings action
        showSettings();
    }

    if (ImGui::Button("Quit"))
    {
        // Handle quit action
        quitGame();
    }

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
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    renderMainMenu();
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

}

void MainMenu::startGame() {
    game.start();
    startGameClicked = false; // Add this line to reset the flag
}

void MainMenu::quitGame() const {
    Game::quit(window);
}

void MainMenu::showSettings() {
    game.settings();
}
