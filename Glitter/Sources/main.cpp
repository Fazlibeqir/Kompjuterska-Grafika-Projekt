// System Headers
#include <GLFW/glfw3.h>

#include "init.h"
#include "MainMenu.h"

int main() {

    const string vertPath= string(SHADER_DIR)+"\\belt.vert";
    const string fragmentPath= string(SHADER_DIR)+ "\\belt.frag";
    const string modelPath= string(MODEL_DIR)+ "\\car\\050 Low Poly Camaro.obj";

    GLFWwindow* mWindow = initializeWindow();
    MainMenu menu(mWindow, vertPath, fragmentPath, modelPath);
    menu.initializeImGui();


//    std::vector<std::string> skyboxTextures = {
//            "assets/img/skybox/right.png",
//            "assets/img/skybox/left.png",
//            "assets/img/skybox/top.png",
//            "assets/img/skybox/bottom.png",
//            "assets/img/skybox/back.png",
//            "assets/img/skybox/front.png"
//    };


    while (!glfwWindowShouldClose(mWindow)) {

        //Load menu
        menu.renderImGui();
        menu.game.processInput(mWindow);

        //Load menu again
        menu.renderImGui();

        // Flip Buffers and Draw
        glfwSwapBuffers(mWindow);
        glfwPollEvents();
    }

    menu.cleanImGui();
    glfwTerminate();
    return 0;
}



