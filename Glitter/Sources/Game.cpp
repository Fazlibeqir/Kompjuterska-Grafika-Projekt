//
// Created by Elena on 09.7.2023.
//

#include <thread>
#include "Game.h"



Game::Game(const std::string& shaderVertPath, const std::string& shaderFragPath,
           const std::string& skyVertPath, const std::string& skyFragPath, const std::string& modelPath)
        : ourShader(shaderVertPath.c_str(),shaderFragPath.c_str()),
        car(modelPath),
         frameBuffer(skyVertPath,skyFragPath),
         camera(glm::vec3(0.0f, 0.0f, 50.0f)),carAcc(5.0f),carTurn(5.0f),
         rotationAngle(0.0f),deltaTime( 0.0f),lastFrame(0.0f),
         ambientS(0.5), diffuseS(1.5),specularS (0.3),
         scale(7.0f)
{

    frameBuffer.frameBufferInitSkyBox();
    frameBuffer.frameBufferInitTextures();
    initShaders();
// Reset the gameStarted state to false in the constructor
    gameStarted = false;
}
void Game::initShaders() {
    ourShader.setInt("material.diffuse",0);
    ourShader.setInt("material.specular",0);
}
void Game::updateDeltaTime()
{
    auto currentFrame = static_cast<float>(glfwGetTime());
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;
}
void Game::setUniforms() {
    ourShader.setVec3("viewPos",camera.position());
    // Set material properties
    ourShader.setFloat("material.shininess",32.0f);
    ourShader.setVec3("dir.direction", lightDirection().x,lightDirection().y,lightDirection().z);
    ourShader.setVec3("dirLight.ambient", ambientS,ambientS,ambientS);
    ourShader.setVec3("dirLight.diffuse", diffuseS,diffuseS,diffuseS);
    ourShader.setVec3("dirLight.specular", specularS,specularS,specularS);
    ourShader.setVec3("spotLight.position",camera.position());
    ourShader.setVec3("spotLight.ambient",1.0f, 1.0f, 1.0f);
    ourShader.setVec3("spotLight.diffuse",1.0f, 1.0f, 1.0f);
    ourShader.setVec3("spotLight.specular",1.0f, 1.0f, 1.0f);
    ourShader.setFloat("spotLight.constant",1.0f);
    ourShader.setFloat("spotLight.linear",0.9f);
    ourShader.setFloat("spotLight.quadratic",0.032);
    ourShader.setFloat("spotLight.cuOff",glm::cos(glm::radians(12.5f)));
    ourShader.setFloat("spotLight.outerCutOff",glm::cos(glm::radians(15.0f)));

}
void Game::initialStart(){

    updateDeltaTime();

    if (!gameStarted) {
        // The game just started, reset the rotation angle to 0 and set the gameStarted flag to true
        rotationAngle += 45.0f * deltaTime; // Adjust the rotation speed as desired
        // Background Fill Color
        glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glBindTexture(GL_TEXTURE_2D, frameBuffer.texture);
        ourShader.use();

        ourShader.setFloat("model", 1);

        glBindVertexArray(frameBuffer.VAO);
        glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_INT, nullptr);

        setUniforms();

        frameBuffer.projection = glm::perspective(glm::radians(camera.zoom()),
                                      (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        frameBuffer.view = camera.GetViewMatrix();
        ourShader.setMat4("projection", frameBuffer.projection);
        ourShader.setMat4("view", frameBuffer.view);

        frameBuffer.model = glm::mat4(1.0f);
        frameBuffer.model = glm::translate(frameBuffer.model, glm::vec3(0.0f, -3.0f, 0.0f));
        frameBuffer.model = glm::scale(frameBuffer.model, glm::vec3(scale));
        frameBuffer.model = glm::rotate(frameBuffer.model, glm::radians(rotationAngle),
                            glm::vec3(0.0f, 1.0f, 0.0f)); // Rotate around the y-axis
        ourShader.setMat4("model", frameBuffer.model);

        car.Draw(ourShader);

        //we can draw more models
        frameBuffer.frameBufferRenderSkyBox();

        if (rotationAngle >= 360.0f) {
            rotationAngle = 0.0f;
        }
    }
}

void Game::start(GLFWwindow* window) {
    updateDeltaTime();

    int windowWidth, windowHeight;
    glfwGetWindowSize(window, &windowWidth, &windowHeight);

    // Calculate camera position and orientation to follow the car
    glm::vec3 carPosition = glm::vec3(carTurn, -12.0f, carAcc);
    glm::vec3 cameraOffset = glm::vec3(0.0f, 5.0f, -10.0f); // Adjust the offset as needed
    glm::vec3 cameraPosition = carPosition + cameraOffset;
    glm::vec3 cameraTarget = carPosition; // Look at the car's position

    // Calculate the minimum and maximum camera positions based on window dimensions
    const float halfWindowWidth = windowWidth / 2.0f;
    const float halfWindowHeight = windowHeight / 2.0f;
    const float minX = -halfWindowWidth;
    const float maxX = halfWindowWidth;
    const float minY = -halfWindowHeight;
    const float maxY = halfWindowHeight;

    // Clamp camera position to stay within window bounds
    cameraPosition.x = glm::clamp(cameraPosition.x, minX, maxX);
    cameraPosition.y = glm::clamp(cameraPosition.y, minY, maxY);

    // Set the view matrix for all shaders
    frameBuffer.view = glm::lookAt(cameraPosition, cameraTarget, glm::vec3(0.0f, 1.0f, 0.0f));
    ourShader.setMat4("view", frameBuffer.view);
    frameBuffer.skyboxShader.setMat4("view", frameBuffer.view);


    // Background Fill Color
    glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Set the projection matrix (assuming this is constant throughout the scene)
    frameBuffer.projection = glm::perspective(glm::radians(90.0f),
                                            (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
    ourShader.setMat4("projection", frameBuffer.projection);
    frameBuffer.skyboxShader.setMat4("projection", frameBuffer.projection);




    // Draw the scene
    glBindTexture(GL_TEXTURE_2D, frameBuffer.texture);
    ourShader.use();
    frameBuffer.model = glm::mat4(1.0f);

    glBindVertexArray(frameBuffer.VAO);
    glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_INT, nullptr);

    scale = 12.0f;

    setUniforms();

    // Draw the car
    frameBuffer.model = glm::mat4(1.0f);
    frameBuffer.model = glm::translate(frameBuffer.model, glm::vec3(carTurn, -12.0f, carAcc));
    frameBuffer.model = glm::rotate(frameBuffer.model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    frameBuffer.model = glm::scale(frameBuffer.model, glm::vec3(scale));
    ourShader.setMat4("model", frameBuffer.model);
    car.Draw(ourShader);

    // Draw the skybox
    frameBuffer.frameBufferRenderSkyBox();

}

void Game::quit(GLFWwindow* window){
    glfwSetWindowShouldClose(window, GLFW_TRUE);
}
void Game::settings(){

}

void Game::processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS){
        // Toggle menu visibility when the Escape key is pressed
        returnToMenuClicked = !returnToMenuClicked;

        // You may want to add additional logic here, such as pausing the game
        // when the menu is visible and resuming when it's hidden.

        // Wait a short duration to prevent multiple toggles on a single key press
        std::this_thread::sleep_for(std::chrono::milliseconds(200));
    }
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS){
        carAcc-=1.0f;
        carAcc = glm::clamp(carAcc, -33.0f, -4.0f);
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS){
        carAcc+=1.0f;
        carAcc = glm::clamp(carAcc, -33.0f, -4.0f);
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS){
        carTurn+=1.0f;
        carTurn = glm::clamp(carTurn, -17.0f, 15.0f);
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS){
        carTurn-=1.0f;
        carTurn = glm::clamp(carTurn, -17.0f, 15.0f);
    }

}

glm::vec3 Game::lightDirection() {
    return {-10.2f, 0.0f, -5.0f};
}

void Game::setRotationAngle(){
    rotationAngle = 0.0f;
}

