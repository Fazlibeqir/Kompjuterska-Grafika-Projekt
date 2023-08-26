#include "Shader.hpp"
#include "init.h"
#include "Car.h"
#include "Terrain.h"
#include "GlobalVariables.h"
#include "Game.h"
#include "Audio.hpp"
#include "MainMenu.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <cstdlib>
#include <string>

int main() {
    GLFWwindow *window = Init::initializeWindow();
    map<const string, string> mapForPaths = Init::initializeShadersAndModelsPaths();

    MainMenu mainMenu(window);
    mainMenu.initializeImGui();
    mainMenu.show();
    Game game(mapForPaths["carVertPath"],mapForPaths["carFragPath"],
              mapForPaths["carOneModelPath"],
              mapForPaths["carTwoModelPath"],
              mapForPaths["carThreeModelPath"],
              mapForPaths["tyre1ModelPath"],
              mapForPaths["tyre2ModelPath"],
              mapForPaths["terrainVertPath"], mapForPaths["terrainFragPath"],
              mapForPaths["terrainModel1Path"], mapForPaths["terrainModel2Path"],mapForPaths["terrainModel3Path"],
              mapForPaths["skyVertPath"],mapForPaths["skyFragPath"]);
    game.initialize();
   // Audio audio;

    enum GameState { MENU, GAME };
    GameState gameState = MENU;
    bool stateChanged = false;

    while (!glfwWindowShouldClose(window)) {
        Init::updateDeltaTime();
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

//        game.startGame(window);
    if(gameState==GAME){
        Init::processInput(window);
        game.simulation.updateMovements();
        // Step physics forward
        game.simulation.dynamicsWorld->stepSimulation((
                GlobalVariables::deltaTime < GlobalVariables::maxSecPerFrame ?
                GlobalVariables::deltaTime : GlobalVariables::maxSecPerFrame), 10);

        game.updateCameraPosition();
        game.transform();
        // Transforms

        glm::mat4 objModelMatrix;
        glm::mat3 objNormalMatrix;

        GLfloat matrix[16];
        btTransform transform;

        glm::vec3 obj_size(1.0f);
        Model *objectModel;

        int num_cobjs = game.simulation.dynamicsWorld->getNumCollisionObjects();

        for (unsigned int i = GlobalVariables::tiles + GlobalVariables::walls; i < num_cobjs; i++) {
            switch (i) {
                case GlobalVariables::tiles + GlobalVariables::walls:
                    objectModel = &game.cars[game.chosenCarIndex].carModel;
                    break;
                case GlobalVariables::tiles + GlobalVariables::walls + 1:
                case GlobalVariables::tiles + GlobalVariables::walls + 2:
                    objectModel = &game.cars[game.chosenCarIndex].tyre1Model;
                    break;
                case GlobalVariables::tiles + GlobalVariables::walls + 3:
                case GlobalVariables::tiles + GlobalVariables::walls + 4:
                    objectModel = &game.cars[game.chosenCarIndex].tyre2Model;
                    break;
                default:
                    return (EXIT_FAILURE);
            }
            // taking the Collision Object from the list
            btCollisionObject *obj = game.simulation.dynamicsWorld->getCollisionObjectArray()[i];
            btRigidBody *body = btRigidBody::upcast(obj);

            // transformation matrix of the rigid body, as calculated by the physics engine
            body->getMotionState()->getWorldTransform(transform);

            //Bullet matrix (transform) to an array of floats
            transform.getOpenGLMatrix(matrix);

            //GLM transformation matrix
            objModelMatrix = glm::make_mat4(matrix) * glm::scale(objModelMatrix, obj_size);
            objNormalMatrix = glm::transpose(glm::inverse(glm::mat3(objModelMatrix)));

            //normal matrix
            glUniformMatrix4fv(glGetUniformLocation(game.cars[game.chosenCarIndex].carShader.Program, "model"), 1, GL_FALSE,
                               glm::value_ptr(objModelMatrix));
            glUniformMatrix3fv(glGetUniformLocation(game.cars[game.chosenCarIndex].carShader.Program, "normal"), 1, GL_FALSE,
                               glm::value_ptr(objNormalMatrix));

            game.cars[game.chosenCarIndex].carShader.setVec3("lightColor", glm::vec3(1.0));
            game.cars[game.chosenCarIndex].carShader.setVec3("lightPos", GlobalVariables::lightPos);
            game.cars[game.chosenCarIndex].carShader.setVec3("viewPos", GlobalVariables::camera.Position);

            game.cars[game.chosenCarIndex].carShader.setFloat("material.shininess", 128.0f);

            game.cars[game.chosenCarIndex].carShader.setVec3("light.direction", 1.0f, -0.5f, -0.5f);
            game.cars[game.chosenCarIndex].carShader.setVec3("light.ambient", 0.5f, 0.5f, 0.5f);
            game.cars[game.chosenCarIndex].carShader.setVec3("light.diffuse", 0.945f, 0.855f, 0.643f);
            game.cars[game.chosenCarIndex].carShader.setVec3("light.specular", 4.0f, 4.0f, 4.0f);

            glActiveTexture(GL_TEXTURE3);
            game.cars[game.chosenCarIndex].carShader.setInt("skybox", 3);
            glBindTexture(GL_TEXTURE_CUBE_MAP, game.cubeMapTexture);

            objectModel->Draw(game.cars[game.chosenCarIndex].carShader);

            objModelMatrix = glm::mat4(1.0f);
            objNormalMatrix = glm::mat4(1.0f);
        }

        game.view = glm::mat4(glm::mat3(GlobalVariables::camera.GetViewMatrix()));

        // Skybox
        game.setSkybox();
        mainMenu.renderImGui();
        if (GlobalVariables::returnToMenuClicked) {
            // Transition back to the main menu
            mainMenu.gameStarted = false;
            mainMenu.show();
        }else{
            mainMenu.gameStarted = true;
            mainMenu.hide();
        }

    }else{
        Init:: processInputForPreGame(window, game.chosenCarIndex);
        
        game.rotationAngle+=45.0f*GlobalVariables::deltaTime;
        //std::cout << "Rotation Angle: " << game.rotationAngle << std::endl;
        game.preGame();

        glm::mat4 objModelMatrix;
        glm::mat3 objNormalMatrix;

        GLfloat matrix[16];
        btTransform transform;

        glm::vec3 obj_size(1.0f);
        Model *objectModel;
        btRigidBody *tireBody = nullptr;

        int num_cobjs = game.simulation.dynamicsWorld->getNumCollisionObjects();

        for (unsigned int i = GlobalVariables::tiles + GlobalVariables::walls; i < num_cobjs; i++) {
            switch (i) {
                case GlobalVariables::tiles + GlobalVariables::walls:
                    objectModel = &game.cars[game.chosenCarIndex].carModel;
                    break;
                case GlobalVariables::tiles + GlobalVariables::walls + 1:
                case GlobalVariables::tiles + GlobalVariables::walls + 2:
                    objectModel = &game.cars[game.chosenCarIndex].tyre1Model;
                    tireBody = game.simulation.t1;
                    break;
                case GlobalVariables::tiles + GlobalVariables::walls + 3:
                case GlobalVariables::tiles + GlobalVariables::walls + 4:
                    objectModel = &game.cars[game.chosenCarIndex].tyre2Model;
                    tireBody = game.simulation.t2;
                    break;
                case GlobalVariables::tiles + GlobalVariables::walls + 5:
                    tireBody = game.simulation.t3; // Correct the tire index
                    break;
                case GlobalVariables::tiles + GlobalVariables::walls + 6:
                    tireBody = game.simulation.t4; // Correct the tire index
                    break;
                default:
                    return (EXIT_FAILURE);
            }
            // taking the Collision Object from the list
            btCollisionObject *obj = game.simulation.dynamicsWorld->getCollisionObjectArray()[i];
            btRigidBody *body = btRigidBody::upcast(obj);

            // transformation matrix of the rigid body, as calculated by the physics engine
            body->getMotionState()->getWorldTransform(transform);

            //Rotation XD
            btQuaternion rotation;
            rotation.setRotation(btVector3(0.0f,1.0f,0.0f),glm::radians(game.rotationAngle));
            transform.setRotation(rotation);
            //Bullet matrix (transform) to an array of floats
            transform.getOpenGLMatrix(matrix);

            //GLM transformation matrix
            objModelMatrix = glm::make_mat4(matrix) * glm::scale(objModelMatrix, obj_size);
            objNormalMatrix = glm::transpose(glm::inverse(glm::mat3(objModelMatrix)));

            //normal matrix
            glUniformMatrix4fv(glGetUniformLocation(game.cars[game.chosenCarIndex].carShader.Program, "model"), 1, GL_FALSE,
                               glm::value_ptr(objModelMatrix));
            glUniformMatrix3fv(glGetUniformLocation(game.cars[game.chosenCarIndex].carShader.Program, "normal"), 1, GL_FALSE,
                               glm::value_ptr(objNormalMatrix));

            game.cars[game.chosenCarIndex].carShader.setVec3("lightColor", glm::vec3(1.0));
            game.cars[game.chosenCarIndex].carShader.setVec3("lightPos", GlobalVariables::lightPos);
            game.cars[game.chosenCarIndex].carShader.setVec3("viewPos", GlobalVariables::camera.Position);

            game.cars[game.chosenCarIndex].carShader.setFloat("material.shininess", 128.0f);

            game.cars[game.chosenCarIndex].carShader.setVec3("light.direction", 1.0f, -0.5f, -0.5f);
            game.cars[game.chosenCarIndex].carShader.setVec3("light.ambient", 0.5f, 0.5f, 0.5f);
            game.cars[game.chosenCarIndex].carShader.setVec3("light.diffuse", 0.945f, 0.855f, 0.643f);
            game.cars[game.chosenCarIndex].carShader.setVec3("light.specular", 4.0f, 4.0f, 4.0f);

            glActiveTexture(GL_TEXTURE3);
            game.cars[game.chosenCarIndex].carShader.setInt("skybox", 3);
            glBindTexture(GL_TEXTURE_CUBE_MAP, game.cubeMapTexture);

            objectModel->Draw(game.cars[game.chosenCarIndex].carShader);

            objModelMatrix = glm::mat4(1.0f);
            objNormalMatrix = glm::mat4(1.0f);
        }
        game.view = glm::mat4(glm::mat3(GlobalVariables::camera.GetViewMatrix()));

        // Skybox
        game.setSkybox();
        mainMenu.renderImGui();
        if(mainMenu.gameStarted){
            gameState=GAME;
        }
    }

        // Flip Buffers and Draw
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    MainMenu::cleanImGui();
    glfwTerminate();
    return EXIT_SUCCESS;
}
