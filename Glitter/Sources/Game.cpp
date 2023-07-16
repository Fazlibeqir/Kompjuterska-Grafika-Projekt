//
// Created by Elena on 09.7.2023.
//

#include "Game.h"
#include "init.h"

Game::Game(const std::string& shaderVertPath, const std::string& shaderFragPath, const std::string& modelPath)
        :ourShader(shaderVertPath.c_str(),shaderFragPath.c_str()),
        car(modelPath.c_str()),
        camera(glm::vec3(0.0f, 0.0f, 50.0f))
    {
        rotationAngle = 0.0f;
        deltaTime = 0.0f;
        lastFrame = 0.0f;
    }

void Game::initialStart(){

    float currentFrame = static_cast<float>(glfwGetTime());
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;

    rotationAngle += 45.0f * deltaTime; // Adjust the rotation speed as desired

    // Background Fill Color
    glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    ourShader.use();
    float time = glfwGetTime();
    glm::vec3 light_pos = glm::vec3(7.0f * cos(time), 10.0f, 7.0f * sin(time));
    glm::vec3 light_pos_ops = glm::vec3(-7.0f * cos(time), 10.0f, -7.0f * sin(time));

    glm::mat4 projection = glm::perspective(glm::radians(camera.zoom()),
                                            (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
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

}

void Game::start(){

}
void Game::quit(GLFWwindow* window){
    glfwSetWindowShouldClose(window, GLFW_TRUE);
}
void Game::settings(){

}

void Game::processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

}