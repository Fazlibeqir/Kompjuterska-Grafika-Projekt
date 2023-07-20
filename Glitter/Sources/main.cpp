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
   //    Light light;

    while (!glfwWindowShouldClose(mWindow)) {


        Game::processInput(mWindow);

        //Load menu
        menu.renderImGui();


        // Flip Buffers and Draw
        glfwSwapBuffers(mWindow);
        glfwPollEvents();
    }

    MainMenu::cleanImGui();
    glfwTerminate();
    return 0;
}



