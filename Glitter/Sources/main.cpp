#include "init.h"
#include "MainMenu.h"

#include <iostream>
using namespace std;



int main() {
    const string vertPath= string(SHADER_DIR)+"\\default.vert";
    const string fragmentPath= string(SHADER_DIR)+ "\\default.frag";
    const string skyVertPath= string(SHADER_DIR)+"\\skybox.vert";
    const string skyFragPath= string(SHADER_DIR)+"\\skybox.frag";
    const string heightVertPath= string(SHADER_DIR)+"\\terrian.vert";
    const string heightFragPath= string(SHADER_DIR)+"\\terrian.frag";
    const string modelPath= string(MODEL_DIR)+ "\\car\\car.obj";
    const string modelPaths[] = { string(MODEL_DIR)+"\\car\\050 Low Poly Camaro.obj", string(MODEL_DIR)+"\\car\\car.obj" };

    GLFWwindow* mWindow = initializeWindow();
    MainMenu menu(mWindow,vertPath,fragmentPath,skyVertPath,skyFragPath,heightVertPath,heightFragPath,modelPaths);
    menu.initializeImGui();
    menu.show();
   //    Light light;

    enum GameState { MENU, GAME };
    GameState gameState = MENU;
    bool stateChanged = false;

    while (!glfwWindowShouldClose(mWindow)) {

        processInput(mWindow, &menu.game.chosenCarIndex, menu.game.gameStarted);

        if (gameState == GAME) {

            // Game state
            menu.game.start( mWindow);
            menu.renderImGui();

            // Check if the game is finished or if the player wants to go back to the main menu
            if (returnToMenuClicked) {
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



