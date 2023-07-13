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

    string vertPath= string(SHADER_DIR)+"\\deferred.vert";
    string fragmentPath= string(SHADER_DIR)+ "\\deferred.frag";
    string modelPath= string(MODEL_DIR)+ "\\car\\050 Low Poly Camaro.obj";

    Shader ourShader(vertPath.c_str(),fragmentPath.c_str());
    Model car(modelPath.c_str());
    Light light;



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

        ourShader.setVec3("viewPos", camera.position());
        ourShader.setVec3("dirLight.direction", -0.2f, -1.0f, -0.3f);
        ourShader.setVec3("dirLight.ambient", 0.05f, 0.05f, 0.05f);
        ourShader.setVec3("dirLight.diffuse", 0.4f, 0.4f, 0.4f);
        ourShader.setVec3("dirLight.specular", 0.5f, 0.5f, 0.5f);
        ourShader.setVec3("pointLights[0].position", light_pos);
        ourShader.setVec3("pointLights[0].ambient", 0.05f, 0.05f, 0.05f);
        ourShader.setVec3("pointLights[0].diffuse", 0.8f, 0.8f, 0.8f);
        ourShader.setVec3("pointLights[0].specular", 1.0f, 1.0f, 1.0f);
        ourShader.setFloat("pointLights[0].constant", 1.0f);
        ourShader.setFloat("pointLights[0].linear", 0.007f);
        ourShader.setFloat("pointLights[0].quadratic", 0.0002f);
        ourShader.setVec3("pointLights[1].position", light_pos_ops);
        ourShader.setVec3("pointLights[1].ambient", 0.05f, 0.05f, 0.05f);
        ourShader.setVec3("pointLights[1].diffuse", 0.8f, 0.8f, 0.8f);
        ourShader.setVec3("pointLights[1].specular", 1.0f, 1.0f, 1.0f);
        ourShader.setFloat("pointLights[1].constant", 1.0f);
        ourShader.setFloat("pointLights[1].linear", 0.007f);
        ourShader.setFloat("pointLights[1].quadratic", 0.0002f);

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



