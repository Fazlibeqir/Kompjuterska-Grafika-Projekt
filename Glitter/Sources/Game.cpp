//
// Created by Elena on 09.7.2023.
//

#include <thread>
#include "Game.h"



Game::Game(const std::string& shaderVertPath, const std::string& shaderFragPath,
           const std::string& skyVertPath, const std::string& skyFragPath,
           const std::string& heightVertPath, const std::string& heightFragPath,
           const std::string modelPaths[])
        :  ourShader(shaderVertPath.c_str(), shaderFragPath.c_str()),
        cars{Model(modelPaths[0]), Model(modelPaths[1])},
          frameBuffer(skyVertPath, skyFragPath, heightVertPath, heightFragPath),
          camera(glm::vec3(0.0f, 0.0f, 50.0f)),
          chosenCarIndex(1),
          dynamicsWorld(nullptr), carPhysics(nullptr, modelPaths[1]), ambientS(0.5),
          diffuseS(1.5), specularS(0.3), scale(7.0f), score(0) {
    carPhysics.initialize(modelPaths);
    frameBuffer.frameBufferInitSkyBox();
    frameBuffer.frameBufferInitTextures();
    frameBuffer.frameBufferInitTerrian();
    initShaders();
    // Reset the gameStarted state to false in the constructor
    gameStarted = false;
}

void Game::initShaders() {
    ourShader.setInt("material.diffuse",0);
    ourShader.setInt("material.specular",0);
}
void Game::setUniforms(Shader& shader) {
    shader.setVec3("viewPos",camera.Position);
    // Set material properties
    shader.setFloat("material.shininess",32.0f);
    shader.setVec3("dir.direction", lightDirection().x,lightDirection().y,lightDirection().z);
    shader.setVec3("dirLight.ambient", ambientS,ambientS,ambientS);
    shader.setVec3("dirLight.diffuse", diffuseS,diffuseS,diffuseS);
    shader.setVec3("dirLight.specular", specularS,specularS,specularS);
    shader.setVec3("spotLight.position",camera.Position);
    shader.setVec3("spotLight.ambient",1.0f, 1.0f, 1.0f);
    shader.setVec3("spotLight.diffuse",1.0f, 1.0f, 1.0f);
    shader.setVec3("spotLight.specular",1.0f, 1.0f, 1.0f);
    shader.setFloat("spotLight.constant",1.0f);
    shader.setFloat("spotLight.linear",0.9f);
    shader.setFloat("spotLight.quadratic",0.032);
    shader.setFloat("spotLight.cuOff",glm::cos(glm::radians(12.5f)));
    shader.setFloat("spotLight.outerCutOff",glm::cos(glm::radians(15.0f)));

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

        setUniforms(ourShader);

        frameBuffer.projection = glm::perspective(glm::radians(camera.Zoom),
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

        cars[chosenCarIndex].Draw(ourShader);

        //we can draw more models
        frameBuffer.frameBufferRenderSkyBox();

        if (rotationAngle >= 360.0f) {
            rotationAngle = 0.0f;
        }
    }
}

void Game::start(GLFWwindow* window) {
    updateDeltaTime();
    carPhysics.update(deltaTime);

    // Calculate camera position and orientation to follow the car
    carPosition = carPhysics.getCarPosition(); // Initialize the car position at the origin with a y-offset of -12.0f
    glm::vec3 cameraOffset = glm::vec3(0.0f, 11.5f, 35.0f); // Adjust the offset as needed
    glm::vec3 cameraPosition = carPosition + cameraOffset;
    glm::vec3 cameraTarget = carPosition; // Look at the car's position


    // Set the view matrix for all shaders
    frameBuffer.view = glm::lookAt(cameraPosition, cameraTarget, glm::vec3(0.0f, 1.0f, 0.0f));
    ourShader.setMat4("view", frameBuffer.view);

    glm::vec3 skyboxCameraPosition= carPosition+glm::vec3(0.0f,1.0f,0.0f);
    frameBuffer.skyboxView= glm::lookAt(skyboxCameraPosition, carPosition, glm::vec3(0.0f, 1.0f, 0.0f));
    frameBuffer.skyboxShader.setMat4("view",frameBuffer.skyboxView);


    // Background Fill Color
    glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Set the projection matrix (assuming this is constant throughout the scene)
    frameBuffer.projection = glm::perspective(glm::radians(90.0f),
                                            (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
    ourShader.setMat4("projection", frameBuffer.projection);


    frameBuffer.skyboxModel=glm::translate(glm::mat4(1.0f),carPosition);
    frameBuffer.skyboxModel=glm::scale(frameBuffer.skyboxModel,glm::vec3(scale));
    frameBuffer.skyboxShader.setMat4("model",frameBuffer.skyboxModel);

    // Draw the skybox
    frameBuffer.frameBufferRenderSkyBox();
    setUniforms(frameBuffer.mapShader);

    frameBuffer.frameBufferRenderTerrian();

    ourShader.use();
    frameBuffer.model = glm::mat4(1.0f);

    scale = 12.0f;
    setUniforms(ourShader);

    // Draw the car
    frameBuffer.model = glm::mat4(1.0f);
    frameBuffer.model = glm::translate(frameBuffer.model, carPosition);
    frameBuffer.model = glm::rotate(frameBuffer.model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    frameBuffer.model = glm::scale(frameBuffer.model, glm::vec3(scale));
    ourShader.setMat4("model", frameBuffer.model);
    cars[chosenCarIndex].Draw(ourShader);

    scale=7.0f;

    score += 1;
}

void Game::quit(GLFWwindow* window){
    glfwSetWindowShouldClose(window, GLFW_TRUE);
}
void Game::settings(){

}

glm::vec3 Game::lightDirection() {
    return {-10.2f, 0.0f, -5.0f};
}

void Game::setRotationAngle(){
    rotationAngle = 0.0f;
}
void Game::RenderScore()
{
    ImGui::SetNextWindowPos(ImVec2(10, 10)); // Set the position of the ImGui window
    ImGui::Begin("Score"); // Begin a new ImGui window with the title "Score"

    // Render the score text using ImGui::Text
    ImGui::Text("Score: %d", score);

    ImGui::End(); // End the ImGui window
}