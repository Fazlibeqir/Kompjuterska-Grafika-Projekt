////
//// Created by Elena on 09.7.2023.
////
#include "Game.h"
#include "glm/gtc/type_ptr.hpp"

Game::Game(const string& carShaderVertexPath,
     const string& carShaderFragmentPath,
     const string& carModelPath,
     const string& tyre1ModelPath,
     const string& tyre2ModelPath,
     const string& terrainShaderVertexPath,
     const string& terrainShaderFragmentPath,
     const string& terrainModel1Path,
     const string& terrainModel2Path,
     const string& skyboxShaderVertexPath,
     const string& skyboxShaderFragmentPath)
        :   carForGame(carShaderVertexPath, carShaderFragmentPath, carModelPath, tyre1ModelPath, tyre2ModelPath),
            terrain(terrainShaderVertexPath, terrainShaderFragmentPath, terrainModel1Path, terrainModel2Path),
            skybox(skyboxShaderVertexPath, skyboxShaderFragmentPath),
            simulation()
     {}

void Game::initialize() {
    skybox.generateBuffers(skyboxVAO, skyboxVBO);
    cubeMapTexture = skybox.loadCubeMap();
    simulation.generateTerrain();
    simulation.generateInvisibleWalls();
    simulation.generateCamaro();
    simulation.addConstraints();
}

void Game:: updateCameraPosition(){
    // Update camera position
    if (GlobalVariables::cameraFollow) {
        btTransform temp;
        btVector3 newPos;

        simulation.car->getMotionState()->getWorldTransform(temp);
        float aVelocity = -simulation.car->getAngularVelocity().y();
        newPos = temp.getBasis() * btVector3(
                glm::cos(glm::radians(-10 * glm::sqrt(glm::abs(GlobalVariables::steering))
                * aVelocity + 90 + GlobalVariables::baseYaw / 4)) *
                GlobalVariables:: cameraRadius, 0,
                glm::sin(glm::radians(-10 * glm::sqrt(glm::abs(GlobalVariables::steering))
                * aVelocity + 90 +GlobalVariables:: baseYaw / 4)) *
                GlobalVariables::cameraRadius);

        GlobalVariables:: cameraFollowPos.x = temp.getOrigin().getX() + newPos.x();
        GlobalVariables::cameraFollowPos.y = temp.getOrigin().getY() - glm::sin(glm::radians(GlobalVariables::camera.Pitch))
                * GlobalVariables:: cameraRadius + 1.5;
        GlobalVariables::cameraFollowPos.z = temp.getOrigin().getZ() + newPos.z();

        GlobalVariables:: camera.Position = GlobalVariables::cameraFollowPos;
        GlobalVariables::camera.LookAt(-newPos.x(), newPos.y(), -newPos.z());
    }

}
void Game:: transform(){
    projection = glm::perspective(glm::radians(45.0f),
                                       (float) GlobalVariables::scrWidth / (float) GlobalVariables::scrHeight,
                                       0.1f, 10000.0f);
    view = GlobalVariables::camera.GetViewMatrix();

    terrain.terrainShader.Use();
    terrain.terrainShader.setMat4("projection", projection);
    terrain.terrainShader.setMat4("view", view);
    terrain.terrainShader.setVec3("viewPos", GlobalVariables::camera.Position);

    terrain.terrainShader.setVec3("light.direction", 1.0f, -0.5f, -0.5f);
    terrain.terrainShader.setVec3("light.ambient", 0.473f, 0.428f, 0.322f);
    glm::mat4 model = glm::mat4(1.0f);

    glm::mat4 planeModelMatrix = glm::mat4(1.0f);
    planeModelMatrix = glm::translate(planeModelMatrix, glm::vec3(0.0f, -2.0f, -20.0f));
    planeModelMatrix = glm::rotate(planeModelMatrix,  glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    planeModelMatrix = glm::scale(planeModelMatrix, glm::vec3(2.5f));
    glUniformMatrix4fv(glGetUniformLocation(terrain.terrainShader.Program, "model"), 1, GL_FALSE,
                       glm::value_ptr(planeModelMatrix));

    // Asphalt
    terrain.terrainShader.setFloat("material.shininess", 16.0f);
    terrain.terrainShader.setVec3("light.diffuse", 0.945f, 0.855f, 0.643f);
    terrain.terrainShader.setVec3("light.specular", 2.75f, 2.75f, 2.75f);
    terrain.terrainModel1.Draw(terrain.terrainShader);

    // Grass
    terrain.terrainShader.setFloat("material.shininess", 4.0f);
    terrain.terrainShader.setVec3("light.diffuse", 1.195f, 1.105f, 0.893f);
    terrain.terrainShader.setVec3("light.specular", 1.0f, 1.0f, 1.0f);
    terrain.terrainModel2.Draw(terrain.terrainShader);

    carForGame.carShader.Use();
    carForGame.carShader.setMat4("projection", projection);
    carForGame.carShader.setMat4("view", view);
    model = glm::mat4(1.0f);
    model = glm::translate(model,glm::vec3(0.0f, 1.0f, 0.0f)); // translate at the center of the scene
}

void Game:: setSkybox(){
    // Skybox
    glDepthFunc(GL_LEQUAL);
    skybox.skyBoxShader.Use();
    skybox.skyBoxShader.setMat4("projection", projection);
    skybox.skyBoxShader.setMat4("view", view);
    glBindVertexArray(skyboxVAO);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, cubeMapTexture);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glDepthFunc(GL_LESS);
}