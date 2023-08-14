//
// Created by Elena on 09.7.2023.
//

#include "MainMenu.h"

MainMenu::MainMenu(GLFWwindow* inWindow, const std::string& shaderVertPath, const std::string& shaderFragPath,
                   const std::string& skyVertPath, const std::string& skyFragPath,
                   const std::string& heightVertPath, const std::string& heightFragPath,
                   const std::string modelPaths[])
: window(inWindow), game(shaderVertPath, shaderFragPath,skyVertPath, skyFragPath,heightVertPath,heightFragPath,modelPaths)
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    ImGui::StyleColorsDark();
}

void MainMenu::renderMainMenu(){
    ImGui::Begin("Main Menu");
    ImGui::Separator();
    // Set the button size and style
    ImVec2 buttonSize(200, 40);
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 10));
    if (ImGui::Button("Start Game",buttonSize)|| glfwGetKey(window, GLFW_KEY_ENTER) == GLFW_PRESS)
    {
        // Handle start game action
        game.gameStarted = true;
        hide();

    }else{
        game.gameStarted = false;
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
    if(isMenuVisible() || returnToMenuClicked) {
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        renderMainMenu();

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    }
    else{
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        game.RenderScore();

        ImGui::Render();
    }
}

void MainMenu::quitGame() const {
    Game::quit(window);
}

void MainMenu::showSettings() {
    game.settings();
}
