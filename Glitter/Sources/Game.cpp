////
//// Created by Elena on 09.7.2023.
////
#include "Game.h"
#include "glm/gtc/type_ptr.hpp"
#include "GLFW/glfw3.h"

Game::Game(const string& carShaderVertexPath,
     const string& carShaderFragmentPath,
     const string& carOneModelPath,
     const string& carTwoModelPath,
     const string& tyre1ModelPath,
     const string& tyre2ModelPath,
     const string& terrainShaderVertexPath,
     const string& terrainShaderFragmentPath,
     const string& terrainModel1Path,
     const string& terrainModel2Path,
     const string& terrainModel3Path,
     const string& skyboxShaderVertexPath,
     const string& skyboxShaderFragmentPath)
        :   cars{{carShaderVertexPath, carShaderFragmentPath, carOneModelPath, tyre1ModelPath, tyre2ModelPath },
                 {carShaderVertexPath, carShaderFragmentPath, carTwoModelPath, tyre1ModelPath, tyre2ModelPath}},
            terrain(terrainShaderVertexPath, terrainShaderFragmentPath, terrainModel1Path, terrainModel2Path,terrainModel3Path),
            skybox(skyboxShaderVertexPath, skyboxShaderFragmentPath),
            simulation(),rotationAngle(0.0f), chosenCarIndex(0)
     {
         gameStarted=false;
     }

void Game::initialize() {
    skybox.generateBuffers(skyboxVAO, skyboxVBO);
    cubeMapTexture = skybox.loadCubeMap();
    simulation.generateTerrain();
    simulation.generateInvisibleWalls();
    simulation.generateCamaro();
    simulation.addConstraints();
}
void Game::preGame(){
    if (!gameStarted) {
        updateCameraPosition();
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

                planeModelMatrix = glm::translate(planeModelMatrix, simulation.plane_pos[(GlobalVariables::grid_height)]);
                glUniformMatrix4fv(glGetUniformLocation( terrain.terrainShader.Program, "model"), 1, GL_FALSE,
                                   glm::value_ptr(planeModelMatrix));


                    // Grass
                    terrain.terrainShader.setFloat("material.shininess", 4.0f);
                    terrain.terrainShader.setVec3("light.diffuse", 1.195f, 1.105f, 0.893f);
                    terrain.terrainShader.setVec3("light.specular", 1.0f, 1.0f, 1.0f);
                    terrain.terrainModel3.Draw( terrain.terrainShader);




                planeModelMatrix = glm::mat4(1.0f);

        cars[chosenCarIndex].carShader.Use();
        cars[chosenCarIndex].carShader.setMat4("projection", projection);
        cars[chosenCarIndex].carShader.setMat4("view", view);
        model = glm::mat4(1.0f);
        model = glm::translate(model,glm::vec3(0.0f, 1.0f, 0.0f)); // translate at the center of the scene

        if (rotationAngle >= 360.0f) {
            rotationAngle = 0.0f;
        }
    }
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
    for (unsigned int i = 0; i < GlobalVariables::grid_width; i++) {
        for (unsigned int j = 0; j < GlobalVariables::grid_height; j++) {
            planeModelMatrix = glm::translate(planeModelMatrix, simulation.plane_pos[i * (GlobalVariables::grid_height) + j]);
            glUniformMatrix4fv(glGetUniformLocation( terrain.terrainShader.Program, "model"), 1, GL_FALSE,
                               glm::value_ptr(planeModelMatrix));

            if (GlobalVariables::track[j][i] == 0) {
                // Grass
                 terrain.terrainShader.setFloat("material.shininess", 4.0f);
                terrain.terrainShader.setVec3("light.diffuse", 1.195f, 1.105f, 0.893f);
                terrain.terrainShader.setVec3("light.specular", 1.0f, 1.0f, 1.0f);
                terrain.terrainModel1.Draw( terrain.terrainShader);
            }else if(GlobalVariables::track[j][i]==1){
                //around the race track doing this for speed and car stability purposes
                terrain.terrainShader.setFloat("material.shininess",  4.0f);
                terrain.terrainShader.setVec3("light.diffuse", 1.195f, 1.105f, 0.893f);
                terrain.terrainShader.setVec3("light.specular", 1.0f, 1.0f, 1.0f);
                terrain.terrainModel1.Draw( terrain.terrainShader);
            }

            planeModelMatrix = glm::mat4(1.0f);
        }

    }
    //Race Track
    terrain.terrainShader.setFloat("material.shininess", 16.0f);
    terrain.terrainShader.setVec3("light.diffuse", 0.945f, 0.855f, 0.643f);
    terrain.terrainShader.setVec3("light.specular", 2.75f, 2.75f, 2.75f);
    terrain.terrainModel2.Draw( terrain.terrainShader);

    cars[chosenCarIndex].carShader.Use();
    cars[chosenCarIndex].carShader.setMat4("projection", projection);
    cars[chosenCarIndex].carShader.setMat4("view", view);
    model = glm::mat4(1.0f);
    model = glm::translate(model,glm::vec3(0.0f, 1.0f, 0.0f)); // translate at the center of the scene
    // Asphalt

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