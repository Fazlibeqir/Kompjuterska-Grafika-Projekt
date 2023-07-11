// Local Headers
//#include "glitter.hpp"

// System Headers
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "shader.hpp"
#include "Model.hpp"
#include "init.h"
#include "MainMenu.h"
//#include "light.hpp"


float deltaTime = 0.0f;
float lastFrame = 0.0f;

int main() {

    MainMenu menu;
    GLFWwindow* mWindow = initializeWindow();
    menu.initializeImGui(mWindow);

    string vertPath= string(SHADER_DIR)+"\\belt.vert";
    string fragmentPath= string(SHADER_DIR)+ "\\belt.frag";
    string modelPath= string(MODEL_DIR)+ "\\car\\050 Low Poly Camaro.obj";

    Shader ourShader(vertPath.c_str(),fragmentPath.c_str());
    Model car(modelPath.c_str());


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
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
        glm::mat4 projection = glm::perspective(glm::radians(45.0f),
                                                (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 1000.0f);
        glm::mat4 view = camera.GetViewMatrix();;
        ourShader.use();
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



