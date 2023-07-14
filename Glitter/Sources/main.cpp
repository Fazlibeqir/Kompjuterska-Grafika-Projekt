// Local Headers
//#include "glitter.hpp"

// System Headers
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "shader.hpp"
#include "Model.hpp"
#include "init.h"
#include "MainMenu.h"
#include "light.hpp"
#include <iostream>

float deltaTime = 0.0f;
float lastFrame = 0.0f;

int main() {

    MainMenu menu;
    GLFWwindow* mWindow = initializeWindow();
    menu.initializeImGui(mWindow);

    string vertPath= string(SHADER_DIR)+"\\belt.vert";
    string fragmentPath= string(SHADER_DIR)+ "\\belt.frag";
    string modelPath= string(MODEL_DIR)+ "\\car\\050 Low Poly Camaro.obj";

//    std::vector<std::string> skyboxTextures = {
//            "assets/img/skybox/right.png",
//            "assets/img/skybox/left.png",
//            "assets/img/skybox/top.png",
//            "assets/img/skybox/bottom.png",
//            "assets/img/skybox/back.png",
//            "assets/img/skybox/front.png"
//    };
    Shader ourShader(vertPath.c_str(),fragmentPath.c_str());
    Model car(modelPath.c_str());
//    Light light;



    float rotationAngle = 0.0f; // Initial rotation angle

    while (!glfwWindowShouldClose(mWindow)) {

        //Load menu
        menu.renderImGui();

        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        rotationAngle += 45.0f * deltaTime; // Adjust the rotation speed as desired

        processInput(mWindow);
        // Background Fill Color
        glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
        ourShader.use();
        float time = glfwGetTime();
        glm::vec3 light_pos = glm::vec3(7.0f * cos(time), 10.0f, 7.0f * sin(time));
        glm::vec3 light_pos_ops = glm::vec3(-7.0f * cos(time), 10.0f, -7.0f * sin(time));

        glm::mat4 projection = glm::perspective(glm::radians(camera.zoom()), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        glm::mat4 view = camera.GetViewMatrix();
        ourShader.setMat4("projection", projection);
        ourShader.setMat4("view", view);

        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, -3.0f, 0.0f));
        model = glm::scale(model, glm::vec3(7.0f, 7.0f, 7.0f));
        model = glm::rotate(model, glm::radians(rotationAngle),
                            glm::vec3(0.0f, 1.0f, 0.0f)); // Rotate around the y-axis
        ourShader.setMat4("model", model);

        car.Draw(ourShader);

        //Load menu
        menu.renderImGui();

        // Flip Buffers and Draw
        glfwSwapBuffers(mWindow);
        glfwPollEvents();
    }

    menu.cleanImGui();
    glfwTerminate();
    return 0;
}



