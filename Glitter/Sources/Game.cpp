////
//// Created by Elena on 09.7.2023.
////
#include "Game.h"
#include "glm/gtc/type_ptr.hpp"
#include "GLFW/glfw3.h"

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

void Game::startGame(GLFWwindow *window) {
    simulation.updateMovements();
    // Step physics forward
    simulation.dynamicsWorld->stepSimulation((GlobalVariables::deltaTime < GlobalVariables::maxSecPerFrame ?
                                                GlobalVariables::deltaTime : GlobalVariables::maxSecPerFrame), 10);

    // Update camera position
    if (GlobalVariables::cameraFollow) {
        btTransform temp;
        btVector3 newPos;

        simulation.car->getMotionState()->getWorldTransform(temp);
        float aVelocity = -simulation.car->getAngularVelocity().y();
        newPos = temp.getBasis() * btVector3(
                glm::cos(glm::radians(-10 * glm::sqrt(glm::abs(GlobalVariables::steering)) *
                                aVelocity + 90 + GlobalVariables::baseYaw / 4)) *
                GlobalVariables:: cameraRadius, 0,
                glm::sin(glm::radians(-10 * glm::sqrt(glm::abs(GlobalVariables::steering)) *
                                aVelocity + 90 +GlobalVariables:: baseYaw / 4)) *
                GlobalVariables::cameraRadius);

        GlobalVariables::cameraFollowPos.x = temp.getOrigin().getX() + newPos.x();
        GlobalVariables::cameraFollowPos.y = temp.getOrigin().getY() -
                glm::sin(glm::radians(GlobalVariables::camera.Pitch)) * GlobalVariables::cameraRadius + 1.5;
        GlobalVariables::cameraFollowPos.z = temp.getOrigin().getZ() + newPos.z();

        GlobalVariables::camera.Position = GlobalVariables::cameraFollowPos;
        GlobalVariables::camera.LookAt(-newPos.x(), newPos.y(), -newPos.z());
    }

    // Transforms
    glm::mat4 projection = glm::perspective(glm::radians(45.0f),
                                            (float) GlobalVariables::scrWidth / (float) GlobalVariables::scrHeight,
                                            0.1f, 10000.0f);
    glm::mat4 view = GlobalVariables::camera.GetViewMatrix();

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
            planeModelMatrix = glm::translate(planeModelMatrix,
                                              simulation.plane_pos[i * (GlobalVariables::grid_height) + j]);
            glUniformMatrix4fv(glGetUniformLocation( terrain.terrainShader.Program, "model"), 1, GL_FALSE,
                               glm::value_ptr(planeModelMatrix));

            if (GlobalVariables::track[j][i] == 0) {
                // Grass
                terrain.terrainShader.setFloat("material.shininess", 4.0f);
                terrain.terrainShader.setVec3("light.diffuse", 1.195f, 1.105f, 0.893f);
                terrain.terrainShader.setVec3("light.specular", 1.0f, 1.0f, 1.0f);
                terrain.terrainModel1.Draw( terrain.terrainShader);
            } else if (GlobalVariables::track[j][i] == 1) {
                // Asphalt
                terrain.terrainShader.setFloat("material.shininess", 16.0f);
                terrain.terrainShader.setVec3("light.diffuse", 0.945f, 0.855f, 0.643f);
                terrain.terrainShader.setVec3("light.specular", 2.75f, 2.75f, 2.75f);
                terrain.terrainModel2.Draw( terrain.terrainShader);
            }

            planeModelMatrix = glm::mat4(1.0f);
        }
    }
    carForGame.carShader.Use();
    carForGame.carShader.setMat4("projection", projection);
    carForGame.carShader.setMat4("view", view);
    model = glm::mat4(1.0f);
    model = glm::translate(model,glm::vec3(0.0f, 1.0f, 0.0f)); // translate at the center of the scene

    glm::mat4 objModelMatrix;
    glm::mat3 objNormalMatrix;

    GLfloat matrix[16];
    btTransform transform;

    glm::vec3 obj_size(1.0f);
    Model *objectModel;

    int num_cobjs = simulation.dynamicsWorld->getNumCollisionObjects();

    for (unsigned int i = GlobalVariables::tiles + GlobalVariables::walls; i < num_cobjs; i++) {
        switch (i) {
            case GlobalVariables::tiles + GlobalVariables::walls:
                objectModel = &carForGame.carModel;
                break;
            case GlobalVariables::tiles + GlobalVariables::walls + 1:
            case GlobalVariables::tiles + GlobalVariables::walls + 2:
                objectModel = &carForGame.tyre1Model;
                break;
            case GlobalVariables::tiles + GlobalVariables::walls + 3:
            case GlobalVariables::tiles + GlobalVariables::walls + 4:
                objectModel = &carForGame.tyre2Model;
                break;
            default:
                exit(1);
        }
        // taking the Collision Object from the list
        btCollisionObject *obj = simulation.dynamicsWorld->getCollisionObjectArray()[i];
        btRigidBody *body = btRigidBody::upcast(obj);

        // transformation matrix of the rigid body, as calculated by the physics engine
        body->getMotionState()->getWorldTransform(transform);

        //Bullet matrix (transform) to an array of floats
        transform.getOpenGLMatrix(matrix);

        //GLM transformation matrix
        objModelMatrix = glm::make_mat4(matrix) * glm::scale(objModelMatrix, obj_size);
        objNormalMatrix = glm::transpose(glm::inverse(glm::mat3(objModelMatrix)));

        //normal matrix
        glUniformMatrix4fv(glGetUniformLocation(carForGame.carShader.Program, "model"), 1, GL_FALSE,
                           glm::value_ptr(objModelMatrix));
        glUniformMatrix3fv(glGetUniformLocation(carForGame.carShader.Program, "normal"), 1, GL_FALSE,
                           glm::value_ptr(objNormalMatrix));

        carForGame.carShader.setVec3("lightColor", glm::vec3(1.0));
        carForGame.carShader.setVec3("lightPos", GlobalVariables::lightPos);
        carForGame.carShader.setVec3("viewPos", GlobalVariables::camera.Position);

        carForGame.carShader.setFloat("material.shininess", 128.0f);

        carForGame.carShader.setVec3("light.direction", 1.0f, -0.5f, -0.5f);
        carForGame.carShader.setVec3("light.ambient", 0.5f, 0.5f, 0.5f);
        carForGame.carShader.setVec3("light.diffuse", 0.945f, 0.855f, 0.643f);
        carForGame.carShader.setVec3("light.specular", 4.0f, 4.0f, 4.0f);

        glActiveTexture(GL_TEXTURE3);
        carForGame.carShader.setInt("skybox", 3);
        glBindTexture(GL_TEXTURE_CUBE_MAP, cubeMapTexture);

        objectModel->Draw(carForGame.carShader);

        objModelMatrix = glm::mat4(1.0f);
        objNormalMatrix = glm::mat4(1.0f);
    }

    view = glm::mat4(glm::mat3(GlobalVariables::camera.GetViewMatrix()));
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

    // Flip Buffers and Draw
    glfwSwapBuffers(window);
    glfwPollEvents();
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
            } else if (GlobalVariables::track[j][i] == 1) {
                // Asphalt
                terrain.terrainShader.setFloat("material.shininess", 16.0f);
                terrain.terrainShader.setVec3("light.diffuse", 0.945f, 0.855f, 0.643f);
                terrain.terrainShader.setVec3("light.specular", 2.75f, 2.75f, 2.75f);
                terrain.terrainModel2.Draw( terrain.terrainShader);
            }

            planeModelMatrix = glm::mat4(1.0f);
        }
    }
    carForGame.carShader.Use();
    carForGame.carShader.setMat4("projection", projection);
    carForGame.carShader.setMat4("view", view);
    model = glm::mat4(1.0f);
    model = glm::translate(model,glm::vec3(0.0f, 1.0f, 0.0f)); // translate at the center of the scene

//    glm::mat4 objModelMatrix;
//    glm::mat3 objNormalMatrix;
//
//    GLfloat matrix[16];
//    btTransform transform;
//
//    glm::vec3 obj_size(1.0f);
//    Model *objectModel;
//
//    int num_cobjs = simulation.dynamicsWorld->getNumCollisionObjects();
//
//    for (unsigned int i = GlobalVariables::tiles + GlobalVariables::walls; i < num_cobjs; i++) {
//        switch (i) {
//            case GlobalVariables::tiles + GlobalVariables::walls:
//                objectModel = &carForGame.carModel;
//                break;
//            case GlobalVariables::tiles + GlobalVariables::walls + 1:
//            case GlobalVariables::tiles + GlobalVariables::walls + 2:
//                objectModel = &carForGame.tyre1Model;
//                break;
//            case GlobalVariables::tiles + GlobalVariables::walls + 3:
//            case GlobalVariables::tiles + GlobalVariables::walls + 4:
//                objectModel = &carForGame.tyre2Model;
//                break;
//            default:
//                exit(1);
//        }
//        // taking the Collision Object from the list
//        btCollisionObject *obj = simulation.dynamicsWorld->getCollisionObjectArray()[i];
//        btRigidBody *body = btRigidBody::upcast(obj);
//
//        // transformation matrix of the rigid body, as calculated by the physics engine
//        body->getMotionState()->getWorldTransform(transform);
//
//        //Bullet matrix (transform) to an array of floats
//        transform.getOpenGLMatrix(matrix);
//
//        //GLM transformation matrix
//        objModelMatrix = glm::make_mat4(matrix) * glm::scale(objModelMatrix, obj_size);
//        objNormalMatrix = glm::transpose(glm::inverse(glm::mat3(objModelMatrix)));
//
//        //normal matrix
//        glUniformMatrix4fv(glGetUniformLocation(carForGame.carShader.Program, "model"), 1, GL_FALSE,
//                           glm::value_ptr(objModelMatrix));
//        glUniformMatrix3fv(glGetUniformLocation(carForGame.carShader.Program, "normal"), 1, GL_FALSE,
//                           glm::value_ptr(objNormalMatrix));
//
//        carForGame.carShader.setVec3("lightColor", glm::vec3(1.0));
//        carForGame.carShader.setVec3("lightPos", GlobalVariables::lightPos);
//        carForGame.carShader.setVec3("viewPos", GlobalVariables::camera.Position);
//
//        carForGame.carShader.setFloat("material.shininess", 128.0f);
//
//        carForGame.carShader.setVec3("light.direction", 1.0f, -0.5f, -0.5f);
//        carForGame.carShader.setVec3("light.ambient", 0.5f, 0.5f, 0.5f);
//        carForGame.carShader.setVec3("light.diffuse", 0.945f, 0.855f, 0.643f);
//        carForGame.carShader.setVec3("light.specular", 4.0f, 4.0f, 4.0f);
//
//        glActiveTexture(GL_TEXTURE3);
//        carForGame.carShader.setInt("skybox", 3);
//        glBindTexture(GL_TEXTURE_CUBE_MAP, cubeMapTexture);
//
//        objectModel->Draw(carForGame.carShader);
//
//        objModelMatrix = glm::mat4(1.0f);
//        objNormalMatrix = glm::mat4(1.0f);
//    }
//
//    view = glm::mat4(glm::mat3(GlobalVariables::camera.GetViewMatrix()));
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