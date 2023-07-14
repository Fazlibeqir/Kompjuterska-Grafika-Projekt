//
// Created by Elena on 09.7.2023.
//

#include "MainMenu.h"

MainMenu::MainMenu() {
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
        startGame();
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
void MainMenu:: initializeImGui(GLFWwindow* window)
{
    this->window =window;
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330 core");

    // Register mouse button callback
//    glfwSetMouseButtonCallback(window, mouseButtonCallback);
//
//    // Register cursor position callback
//    glfwSetCursorPosCallback(window, cursorPosCallback);
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

}

void MainMenu::quitGame() {
    glfwSetWindowShouldClose(window, GLFW_TRUE);
}

void MainMenu::showSettings() {

}
//void MainMenu::mouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
//{
//    if (ImGui::GetIO().WantCaptureMouse)
//        return;
//
//    // Add your code to handle mouse button clicks here
//}
//
//void MainMenu::cursorPosCallback(GLFWwindow* window, double xpos, double ypos)
//{
//    if (ImGui::GetIO().WantCaptureMouse)
//        return;
//
//    // Add your code to handle cursor position updates here
//}