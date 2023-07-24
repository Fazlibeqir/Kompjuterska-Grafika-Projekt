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
    bool stateChanged = false;

    while (!glfwWindowShouldClose(mWindow)) {
        menu.game.processInput(mWindow);

        if (gameState == GAME) {
            // Game state
            menu.game.start( mWindow);

            // Check if the game is finished or if the player wants to go back to the main menu
            if (menu.game.shouldReturnToMenu()) {
                // Transition back to the main menu
                gameState = MENU;
                menu.show();
            }
        } else {
            // Menu state

            // Show the main menu
            menu.game.initialStart();
            menu.renderImGui();

            // Check if the "Start Game" button is clicked in the main menu
            if (menu.game.gameStarted) {
                // Transition to the actual game
                gameState = GAME;
                menu.game.setRotationAngle();
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



