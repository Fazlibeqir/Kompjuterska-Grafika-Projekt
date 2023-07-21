#include "init.h"
#include "MainMenu.h"

#include <iostream>
using namespace std;


int main() {
    const string vertPath= string(SHADER_DIR)+"\\default.vert";
    const string fragmentPath= string(SHADER_DIR)+ "\\default.frag";
    const string skyVertPath= string(SHADER_DIR)+"\\skybox.vert";
    const string skyFragPath= string(SHADER_DIR)+"\\skybox.frag";
    const string modelPath= string(MODEL_DIR)+ "\\car\\050 Low Poly Camaro.obj";

    GLFWwindow* mWindow = initializeWindow();
    MainMenu menu(mWindow,vertPath,fragmentPath,skyVertPath,skyFragPath,modelPath);
    menu.initializeImGui();
    menu.show();
   //    Light light;

    enum GameState { MENU, GAME };
    GameState gameState = MENU;
    bool gameStarted = false;

    while (!glfwWindowShouldClose(mWindow)) {


        menu.game.processInput(mWindow);

        if (gameStarted) {
            // Check if the game is finished or if the player wants to go back to the main menu
            if (menu.game.shouldReturnToMenu()) {
                // Transition back to the main menu
                gameState = MENU;
               // game.cleanup();
                gameStarted = false;
                menu.show();
            }
        } else {
            // Show the main menu

            menu.game.initialStart();
            menu.renderImGui();

            // Check if the "Start Game" button is clicked in the main menu
            if (menu.shouldStartGame()) {
                // Transition to the actual game
                gameState = GAME;
                gameStarted = true;
            }
        }
        // Flip Buffers and Draw
        glfwSwapBuffers(mWindow);
        glfwPollEvents();
    }

    MainMenu::cleanImGui();
    glfwTerminate();
    return 0;
}



