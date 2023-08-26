////
//// Created by Elena on 09.7.2023.
////
#include "Game.h"
#include "glm/gtc/type_ptr.hpp"


Game::Game(const string &carShaderVertexPath,
           const string &carShaderFragmentPath,
           const string& carOneModelPath,
           const string& carTwoModelPath,
           const string& carThreeModelPath,
           const string &tyre1ModelPath,
           const string &tyre2ModelPath,
           const string &terrainShaderVertexPath,
           const string &terrainShaderFragmentPath,
           const string &terrainModel1Path,
           const string &terrainModel2Path,
           const string &terrainModel3Path,
           const string &skyboxShaderVertexPath,
           const string &skyboxShaderFragmentPath)
        : cars{{carShaderVertexPath, carShaderFragmentPath, carOneModelPath, tyre1ModelPath, tyre2ModelPath },
               {carShaderVertexPath, carShaderFragmentPath, carTwoModelPath, tyre1ModelPath, tyre2ModelPath},
               {carShaderVertexPath, carShaderFragmentPath, carThreeModelPath, tyre1ModelPath, tyre2ModelPath} },
               terrain(terrainShaderVertexPath, terrainShaderFragmentPath, terrainModel1Path, terrainModel2Path,
                  terrainModel3Path),
          skybox(skyboxShaderVertexPath, skyboxShaderFragmentPath),
          simulation(), rotationAngle(0.0f),chosenCarIndex(0) {
    gameStarted = false;
}

void Game::initialize() {
    skybox.generateBuffers(skyboxVAO, skyboxVBO);
    cubeMapTexture = skybox.loadCubeMap();
    simulation.generateTerrain();
    simulation.generateInvisibleWalls();
    simulation.generateCamaro();
    simulation.addConstraints();
}
void Game::carUniFrom(glm::mat4& objModelMatrix,glm::mat3& objNormalMatrix) const{
    cars[chosenCarIndex].carShader.setMat4("model",objModelMatrix);
    cars[chosenCarIndex].carShader.setMat3("normal",objNormalMatrix);


    cars[chosenCarIndex].carShader.setVec3("lightColor", glm::vec3(1.0));
    cars[chosenCarIndex].carShader.setVec3("lightPos", GlobalVariables::lightPos);
    cars[chosenCarIndex].carShader.setVec3("viewPos", GlobalVariables::camera.Position);

    cars[chosenCarIndex].carShader.setFloat("material.shininess", 128.0f);

    cars[chosenCarIndex].carShader.setVec3("light.direction", 1.0f, -0.5f, -0.5f);
    cars[chosenCarIndex].carShader.setVec3("light.ambient", 0.5f, 0.5f, 0.5f);
    cars[chosenCarIndex].carShader.setVec3("light.diffuse", 0.945f, 0.855f, 0.643f);
    cars[chosenCarIndex].carShader.setVec3("light.specular", 4.0f, 4.0f, 4.0f);

    cars[chosenCarIndex].carShader.setInt("skybox", 3);
}
void Game::preGame(glm::mat4 &objModelMatrix, glm::mat3 &objNormalMatrix,int& num_cobjs,
                   Model *objectModel, GLfloat *matrix, btTransform transform) {
    glm::vec3 obj_size(1.0f);
    for (unsigned int i = GlobalVariables::tiles + GlobalVariables::walls; i < num_cobjs; i++) {
        switch (i) {
            case GlobalVariables::tiles + GlobalVariables::walls:
                objectModel = &cars[chosenCarIndex].carModel;
                break;
            case GlobalVariables::tiles + GlobalVariables::walls + 1:
            case GlobalVariables::tiles + GlobalVariables::walls + 2:
                objectModel = &cars[chosenCarIndex].tyre1Model;
                break;
            case GlobalVariables::tiles + GlobalVariables::walls + 3:
            case GlobalVariables::tiles + GlobalVariables::walls + 4:
                objectModel = &cars[chosenCarIndex].tyre2Model;
                break;
            default:
                return;
        }
        // taking the Collision Object from the list
        btCollisionObject *obj = simulation.dynamicsWorld->getCollisionObjectArray()[i];
        btRigidBody *body = btRigidBody::upcast(obj);

        // transformation matrix of the rigid body, as calculated by the physics engine
        body->getMotionState()->getWorldTransform(transform);

        //Rotation XD
        btQuaternion rotation;
        rotation.setRotation(btVector3(0.0f, 1.0f, 0.0f), glm::radians(rotationAngle));
        transform.setRotation(rotation);
        //Bullet matrix (transform) to an array of floats
        transform.getOpenGLMatrix(matrix);

        //GLM transformation matrix
        objModelMatrix = glm::make_mat4(matrix) * glm::scale(objModelMatrix, obj_size);
        objNormalMatrix = glm::transpose(glm::inverse(glm::mat3(objModelMatrix)));

        carUniFrom(objModelMatrix, objNormalMatrix);
        glActiveTexture(GL_TEXTURE3);
        glBindTexture(GL_TEXTURE_CUBE_MAP, cubeMapTexture);

        objectModel->Draw(cars[chosenCarIndex].carShader);

        objModelMatrix = glm::mat4(1.0f);
        objNormalMatrix = glm::mat4(1.0f);
    }
    view = glm::mat4(glm::mat3(GlobalVariables::camera.GetViewMatrix()));

    // Skybox
    setSkybox();
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
        terrain.terrainShader.setMat4("model",planeModelMatrix);

        // Grass
        terrain.terrainShader.setFloat("material.shininess", 4.0f);
        terrain.terrainShader.setVec3("light.diffuse", 1.195f, 1.105f, 0.893f);
        terrain.terrainShader.setVec3("light.specular", 1.0f, 1.0f, 1.0f);
        terrain.terrainModel3.Draw(terrain.terrainShader);


        planeModelMatrix = glm::mat4(1.0f);

        cars[chosenCarIndex].carShader.Use();
        cars[chosenCarIndex].carShader.setMat4("projection", projection);
        cars[chosenCarIndex].carShader.setMat4("view", view);
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, 1.0f, 0.0f)); // translate at the center of the scene

        if (rotationAngle >= 360.0f) {
            rotationAngle = 0.0f;
        }
    }
}

void Game::startGame(glm::mat4& objModelMatrix,glm::mat3& objNormalMatrix,int& num_cobjs,
                     Model* objectModel,GLfloat matrix[],btTransform transform){
    glm::vec3 obj_size(1.0f);
    for (unsigned int i = GlobalVariables::tiles + GlobalVariables::walls; i < num_cobjs; i++) {
        switch (i) {
            case GlobalVariables::tiles + GlobalVariables::walls:
                objectModel = &cars[chosenCarIndex].carModel;
                break;
            case GlobalVariables::tiles + GlobalVariables::walls + 1:
            case GlobalVariables::tiles + GlobalVariables::walls + 2:
                objectModel = &cars[chosenCarIndex].tyre1Model;
                break;
            case GlobalVariables::tiles + GlobalVariables::walls + 3:
            case GlobalVariables::tiles + GlobalVariables::walls + 4:
                objectModel = &cars[chosenCarIndex].tyre2Model;
                break;
            default:
                return;
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
        carUniFrom(objModelMatrix, objNormalMatrix);
        glActiveTexture(GL_TEXTURE3);
        glBindTexture(GL_TEXTURE_CUBE_MAP, cubeMapTexture);

        objectModel->Draw(cars[chosenCarIndex].carShader);

        objModelMatrix = glm::mat4(1.0f);
        objNormalMatrix = glm::mat4(1.0f);
    }

    view = glm::mat4(glm::mat3(GlobalVariables::camera.GetViewMatrix()));

    // Skybox
    setSkybox();
}
void Game::updateCameraPosition() const {
    // Update camera position
    if (GlobalVariables::cameraFollow) {
        btTransform temp;
        btVector3 newPos;

        simulation.car->getMotionState()->getWorldTransform(temp);
        float aVelocity = -simulation.car->getAngularVelocity().y();
        newPos = temp.getBasis() * btVector3(
                glm::cos(glm::radians(-10 * glm::sqrt(glm::abs(GlobalVariables::steering))
                                      * aVelocity + 90 + GlobalVariables::baseYaw / 4)) *
                GlobalVariables::cameraRadius, 0,
                glm::sin(glm::radians(-10 * glm::sqrt(glm::abs(GlobalVariables::steering))
                                      * aVelocity + 90 + GlobalVariables::baseYaw / 4)) *
                GlobalVariables::cameraRadius);

        GlobalVariables::cameraFollowPos.x = temp.getOrigin().getX() + newPos.x();
        GlobalVariables::cameraFollowPos.y =
                temp.getOrigin().getY() - glm::sin(glm::radians(GlobalVariables::camera.Pitch))
                                          * GlobalVariables::cameraRadius + 1.5;
        GlobalVariables::cameraFollowPos.z = temp.getOrigin().getZ() + newPos.z();

        GlobalVariables::camera.Position = GlobalVariables::cameraFollowPos;
        GlobalVariables::camera.LookAt(-newPos.x(), newPos.y(), -newPos.z());
    }

}

void Game::transform() {
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
            planeModelMatrix = glm::translate(planeModelMatrix,
                                              simulation.plane_pos[i * (GlobalVariables::grid_height) + j]);
            terrain.terrainShader.setMat4("model",planeModelMatrix);

            if (GlobalVariables::track[j][i] == 0) {
                // Grass
                terrain.terrainShader.setFloat("material.shininess", 4.0f);
                terrain.terrainShader.setVec3("light.diffuse", 1.195f, 1.105f, 0.893f);
                terrain.terrainShader.setVec3("light.specular", 1.0f, 1.0f, 1.0f);
                terrain.terrainModel1.Draw(terrain.terrainShader);
            } else if (GlobalVariables::track[j][i] == 1) {
                //around the race track doing this for speed and car stability purposes
                terrain.terrainShader.setFloat("material.shininess", 4.0f);
                terrain.terrainShader.setVec3("light.diffuse", 1.195f, 1.105f, 0.893f);
                terrain.terrainShader.setVec3("light.specular", 1.0f, 1.0f, 1.0f);
                terrain.terrainModel1.Draw(terrain.terrainShader);
            }

            planeModelMatrix = glm::mat4(1.0f);
        }

    }
    //Race Track
    terrain.terrainShader.setFloat("material.shininess", 16.0f);
    terrain.terrainShader.setVec3("light.diffuse", 0.945f, 0.855f, 0.643f);
    terrain.terrainShader.setVec3("light.specular", 2.75f, 2.75f, 2.75f);
    terrain.terrainModel2.Draw(terrain.terrainShader);

    cars[chosenCarIndex].carShader.Use();
    cars[chosenCarIndex].carShader.setMat4("projection", projection);
    cars[chosenCarIndex].carShader.setMat4("view", view);
    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(0.0f, 1.0f, 0.0f)); // translate at the center of the scene
    // Asphalt

}

void Game::setSkybox() {
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