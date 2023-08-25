//
// Created by Elena on 09.7.2023.
//

#include "MainMenu.h"
#include "Score.hpp"
#include "GlobalVariables.h"


MainMenu::MainMenu(GLFWwindow* inWindow)
: window(inWindow),volume(0.5f){
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    ImGui::StyleColorsDark();
}
void MainMenu::renderMainMenu(){
    int screenWidth, screenHeight;
    const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
    if (mode) {
        screenWidth = mode->width;
        screenHeight = mode->height;
    }

    ImVec2 windowSize(220, 220);
    ImVec2 windowPos(50, 50);  // Desired offset from the top-left corner

    // Adjust window position for screen size
    windowPos.x = static_cast<float>(screenWidth) * windowPos.x / GlobalVariables::scrWidth;
    windowPos.y = static_cast<float>(screenHeight) * windowPos.y / GlobalVariables::scrHeight;

    ImGui::SetNextWindowPos(windowPos);
    ImGui::SetNextWindowSize(windowSize);
    ImGui::Begin("Main Menu",nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove);
    ImGui::Separator();
    // Set the button size and style
    ImVec2 buttonSize(200, 40);
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 10));

    renderMainMenuLogic(buttonSize);

    ImGui::Text("Right or left to choose a car");
    ImGui::PopStyleVar();
    ImGui::End();
}

void MainMenu::renderMainMenuLogic(const ImVec2& buttonSize) {
    // Handle start game action
    if (ImGui::Button("Start Game", buttonSize) || glfwGetKey(window, GLFW_KEY_ENTER) == GLFW_PRESS) {
        if (GlobalVariables::returnToMenuClicked == true)
            GlobalVariables::returnToMenuClicked = !GlobalVariables::returnToMenuClicked;

        gameStarted = true;
        hide();
    } else {
        gameStarted = false;
    }

    // Handle settings action
    if (ImGui::Button("Settings", buttonSize)) {
        showSettingsWindow = !showSettingsWindow; // Toggle visibility
    }

    // Handle quit action
    if (ImGui::Button("Quit", buttonSize)) {
        quitGame();
    }

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
    if(isMenuVisible()||GlobalVariables::returnToMenuClicked) {
        renderMainMenu();
        if (showSettingsWindow) {
            // Handle settings action
            showSettings();
        }
    }
    currentlyPlaying();
    if (gameStarted) {
        renderScoreWindow();
    }
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

}

void MainMenu::quitGame() const {
    audio.~Audio();
    cleanImGui();
    glfwDestroyWindow(window);
    glfwTerminate();
}

void MainMenu::showSettings() {
    ImGui::Begin("Settings");
    ImGui::SliderFloat("Volume", &volume, 0.0f, 1.0f);

    if (ImGui::Button("Apply")) {
        // Apply volume changes to the audio
        audio.setDefaultVolume(volume);
    }

    if (ImGui::Button("Toggle Full Screen")) {
        // Toggle between full screen and windowed mode
        toggleFullScreen();
    }

    ImGui::End();
}
void MainMenu::toggleFullScreen(){
    if (window) {
        int currentMonitor = reinterpret_cast<int>(glfwGetWindowMonitor(window));
        const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());

        if (currentMonitor) {
            // Switch to windowed mode
            glfwSetWindowMonitor(window, nullptr, 100, 100, 960, 540, GLFW_DONT_CARE);
            glViewport(0, 0, 960, 540); // Set the OpenGL viewport for windowed mode
        } else {
            // Switch to full screen mode
            glfwSetWindowMonitor(window, glfwGetPrimaryMonitor(), 0, 0, mode->width, mode->height, mode->refreshRate);
            glViewport(0, 0, mode->width, mode->height); // Set the OpenGL viewport for full screen mode
        }
    }
}
bool MainMenu::hasCrossedFinishLine(const glm::vec3& currentPlayerPosition) {
    // Define the boundaries of the box-like finish line region
    glm::vec3 boxMin = playerPosition - glm::vec3(0.5f);
    glm::vec3 boxMax = playerPosition + glm::vec3(0.5f);

    // Check if the current player position is within the box-like region
    bool withinBox = currentPlayerPosition.x >= boxMin.x && currentPlayerPosition.x <= boxMax.x &&
                     currentPlayerPosition.y >= boxMin.y && currentPlayerPosition.y <= boxMax.y &&
                     currentPlayerPosition.z >= boxMin.z && currentPlayerPosition.z <= boxMax.z;

    // If within the box-like region, increment the crossing count and return true if crossed twice
    if (withinBox) {
        if (crossingCount < 2) {
            crossingCount++;
        }
    }

    return crossingCount >= 2;
}


void MainMenu::setPlayerPosition(const glm::vec3& position) {
    playerPosition = position;
}
void MainMenu::updateRaceStatus() {
    // Check for finish line crossing using playerPosition
    if (!raceFinished && hasCrossedFinishLine(playerPosition)) {
        raceFinished = true;
        finishedRaceTime = 600.0f - countdownTimer;
        // Handle race completion
    }
}
void MainMenu::renderScoreWindow() {
    int screenWidth, screenHeight;
    glfwGetWindowSize(window, &screenWidth, &screenHeight);

    float windowX = (screenWidth - 200) * 0.5f; // Center the window on x-axis
    float windowY = 50.0f;

    ImGui::SetNextWindowPos(ImVec2(windowX, windowY));
    ImGui::SetNextWindowSize(ImVec2(200, 70));
    ImGui::Begin("Score", nullptr, ImGuiWindowFlags_NoResize);

    const std::string scorePath=string(ASSETS_DIR)+"\\highscore.txt";
    // Read the high score from the file
    Score score(scorePath);
    float currentHighScore = score.readHighScore();

    int minutes = static_cast<int>(countdownTimer) / 60;
    int seconds = static_cast<int>(countdownTimer) % 60;
    ImGui::Text("Time left: %02d:%02d", minutes, seconds);
    countdownTimer -= ImGui::GetIO().DeltaTime;

    // Compare with high score and update if necessary
    if (countdownTimer <= 0.0f) {
        if (currentHighScore == 0.0f || currentHighScore > finishedRaceTime) {
            score.updateHighScore(finishedRaceTime);
        }
        countdownTimer = 600.0f; // Reset the timer
    }
//    // Display high score information
//    if (currentHighScore == 0.0f) {
//        ImGui::Text("High Score: No score yet");
//    } else {
//        int highScoreMinutes = static_cast<int>(currentHighScore) / 60;
//        int highScoreSeconds = static_cast<int>(currentHighScore) % 60;
//        ImGui::Text("High Score: %02d:%02d", highScoreMinutes, highScoreSeconds);
//    }

    ImGui::End();
}


void MainMenu::currentlyPlaying() {
    const std::string& songName = audio.getCurrentSongName();

    static bool transitioningIn = true;
    static float transitionOffset = 220.0f; // Initial offset to slide from the right
    static const float transitionSpeed = 0.2f; // Adjust the speed of the transition
    static float showTimer = 0.0f;
    const float showDuration = 5.0f; // Adjust the duration to your preference

    static bool newSongStarted = false; // Indicates if a new song started

    if (!songName.empty()) {
        ImGui::SetNextWindowSize(ImVec2(220, 50)); // Set minimum window size

        // Transitioning in
        if (transitioningIn) {
            transitionOffset -= transitionSpeed;
            if (transitionOffset <= 0.0f) {
                transitioningIn = false; // Stop transitioning in
                showTimer = showDuration; // Start the timer for showing the window
                newSongStarted = true; // Indicate that a new song started
            }
        }

        // Showing
        if (!transitioningIn && showTimer > 0.0f) {

            // Calculate the width of the text
            ImVec2 textSize = ImGui::CalcTextSize(songName.c_str());
            ImVec2 windowSize(textSize.x + 20, 50); // Add padding to the width

            // Calculate the window position based on screen size and desired offset
            int screenWidth, screenHeight;
            glfwGetWindowSize(window, &screenWidth, &screenHeight);
            ImVec2 startPos(screenWidth - windowSize.x-100, 450); // Adjust the X position based on your desired offset

            ImGui::SetNextWindowPos(startPos);
            ImGui::SetNextWindowSize(windowSize);
            ImGui::Begin("Now Playing:", nullptr, ImGuiWindowFlags_NoResize);

            ImGui::Text("%s", songName.c_str());

            ImGui::End();
            showTimer -= ImGui::GetIO().DeltaTime;
        }

        // Transitioning out
        if (!transitioningIn && showTimer <= 0.0f) {
            transitionOffset += transitionSpeed;
            if (transitionOffset >= 220.0f) {
                transitioningIn = true; // Start transitioning in again
                newSongStarted = false; // Reset the new song flag
            }
        }
    } else {
        // Hide the window if no song is playing and a new song started
        if (newSongStarted) {
            transitioningIn = true; // Start transitioning in again
            newSongStarted = false; // Reset the new song flag
        }
    }
}















