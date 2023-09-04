#include "main.hpp"

void handleReturnToMenu(MainMenu &mainMenu) {
    mainMenu.renderImGui();
    if (GlobalVariables::returnToMenuClicked) {
        mainMenu.gameStarted = false;
        mainMenu.show();
    } else {
        mainMenu.gameStarted = true;
        mainMenu.hide();
    }
}
void setUp(GLFWwindow *window, Game &game, Init &init, MainMenu &mainMenu) {
    glm::vec3 playerPosition = game.simulation.getPlayerPosition();
    mainMenu.setPlayerPosition(playerPosition);
    mainMenu.updateRaceStatus();
    if(!mainMenu.disableInputForGame) {
        init.processInputForGame(window);
        game.simulation.updateMovements();
    // Step physics forward
    game.simulation.dynamicsWorld->stepSimulation((
       GlobalVariables::deltaTime < GlobalVariables::maxSecPerFrame ?
       GlobalVariables::deltaTime : GlobalVariables::maxSecPerFrame),10);
    }
    game.updateCameraPosition();
    game.transform();
}
void renderGame(GLFWwindow* window,Game& game,Init& init,MainMenu& mainMenu ){
    setUp(window, game, init, mainMenu);
    glm::mat4 objModelMatrix;
    glm::mat3 objNormalMatrix;
    GLfloat matrix[16];
    btTransform transform;
    Model *objectModel = nullptr;
    int num_cobjs = game.simulation.dynamicsWorld->getNumCollisionObjects();
    game.startGame(objModelMatrix,objNormalMatrix,num_cobjs,objectModel,matrix,transform);
}
void renderPreGame(Game& game,MainMenu& mainMenu ){
    game.rotationAngle += 45.0f * GlobalVariables::deltaTime;
    glm::mat4 objModelMatrix;
    glm::mat3 objNormalMatrix;
    GLfloat matrix[16];
    btTransform transform;
    Model *objectModel= nullptr;

    int num_cobjs = game.simulation.dynamicsWorld->getNumCollisionObjects();
    game.preGame(objModelMatrix,objNormalMatrix,num_cobjs,objectModel,matrix,transform);
    mainMenu.renderImGui();
    if (mainMenu.gameStarted)
        GlobalVariables::gameState = GAME;
}
float skyboxVertices[] =
        {
                //   Coordinates
                -1.0f, -1.0f,  1.0f,//        7--------6
                1.0f, -1.0f,  1.0f,//       /|       /|
                1.0f, -1.0f, -1.0f,//      4--------5 |
                -1.0f, -1.0f, -1.0f,//      | |      | |
                -1.0f,  1.0f,  1.0f,//      | 3------|-2
                1.0f,  1.0f,  1.0f,//      |/       |/
                1.0f,  1.0f, -1.0f,//      0--------1
                -1.0f,  1.0f, -1.0f
        };

unsigned int skyboxIndices[] =
        {
                // Right
                1, 2, 6,
                6, 5, 1,
                // Left
                0, 4, 7,
                7, 3, 0,
                // Top
                4, 5, 6,
                6, 7, 4,
                // Bottom
                0, 3, 2,
                2, 1, 0,
                // Back
                0, 1, 5,
                5, 4, 0,
                // Front
                3, 7, 6,
                6, 2, 3
        };

unsigned int skyboxVAO,skyboxVBO,skyboxEBO;
void intiSkybox(Shader& skyboxShader,Game& game){
    skyboxShader.Use();
    skyboxShader.setInt("skybox",0);
    glGenVertexArrays(1, &skyboxVAO);
    glGenBuffers(1, &skyboxVBO);
    glGenBuffers(1, &skyboxEBO);
    glBindVertexArray(skyboxVAO);
    glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, skyboxEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(skyboxIndices), &skyboxIndices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    string right = string(SKY_DIR) + "\\skybox2\\clouds1_east.bmp";
    string left = string(SKY_DIR) + "\\skybox2\\clouds1_west.bmp";
    string top = string(SKY_DIR) + "\\skybox2\\clouds1_up.bmp";
    string bottom = string(SKY_DIR) + "\\skybox2\\clouds1_down.bmp";
    string back = string(SKY_DIR) + "\\skybox2\\clouds1_south.bmp";
    string front = string(SKY_DIR) + "\\skybox2\\clouds1_north.bmp";
    std::string facesCubemap[6] =
            {
                    right,
                    left,
                    top,
                    bottom,
                    front,
                    back
            };
    glGenTextures(1, &game.cubeMapTexture);
    glBindTexture(GL_TEXTURE_CUBE_MAP, game.cubeMapTexture);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    // These are very important to prevent seams
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    // This might help with seams on some systems
    //glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
    for (unsigned int i = 0; i < 6; i++){
        int width, height, nrChannels;
        unsigned char* data = stbi_load(facesCubemap[i].c_str(), &width, &height, &nrChannels, 0);
        if (data){
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,0,GL_RGB,width,height,0,GL_RGB,GL_UNSIGNED_BYTE,data);
            stbi_image_free(data);
        }
        else{
            std::cout << "Failed to load texture: " << facesCubemap[i] << std::endl;
            stbi_image_free(data);
        }
    }
}
void renderSkybox(Shader& skyboxShader,Game& game){
    // Since the cubemap will always have a depth of 1.0, we need that equal sign so it doesn't get discarded
    glDepthFunc(GL_LEQUAL);

    skyboxShader.Use();
    glm::mat4 view = glm::mat4(1.0f);
    glm::mat4 projection = glm::mat4(1.0f);
    // We make the mat4 into a mat3 and then a mat4 again in order to get rid of the last row and column
    // The last row and column affect the translation of the skybox (which we don't want to affect)
    view = glm::mat4(glm::mat3(glm::lookAt( GlobalVariables::camera.Position,
                                                GlobalVariables::camera.Position + GlobalVariables::camera.Front,
                                                        GlobalVariables::camera.Up)));
    projection = glm::perspective(glm::radians(45.0f), (float)GlobalVariables::scrWidth /
                                                                        GlobalVariables::scrHeight, 0.1f, 100.0f);
    skyboxShader.setMat4("view",view);
    skyboxShader.setMat4("projection",projection);
    // Draws the cubemap as the last object so we can save a bit of performance by discarding all fragments
    // where an object is present (a depth of 1.0f will always fail against any object's depth value)
    glBindVertexArray(skyboxVAO);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, game.cubeMapTexture);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

    // Switch back to the normal depth function
    glDepthFunc(GL_LESS);
}
int main() {
    Init init;
    GLFWwindow *window = Init::initializeWindow();
    map<const string, string> mapForPaths = Init::initializeShadersAndModelsPaths();
    MainMenu mainMenu(window);
    mainMenu.initializeImGui();
    mainMenu.show();
    Game game(mapForPaths["carVertPath"],
              mapForPaths["carFragPath"],
              mapForPaths["carOneModelPath"],
              mapForPaths["carTwoModelPath"],
              mapForPaths["carThreeModelPath"],
              mapForPaths["tyre1ModelPath"],
              mapForPaths["tyre2ModelPath"],
              mapForPaths["terrainVertPath"],
              mapForPaths["terrainFragPath"],
              mapForPaths["terrainModel1Path"],
              mapForPaths["terrainModel2Path"],
              mapForPaths["terrainModel3Path"]
              );
    game.initialize();
    Shader skyboxShader(mapForPaths["skyVertPath"].c_str(), mapForPaths["skyFragPath"].c_str());
    intiSkybox(skyboxShader,game);

    while (!glfwWindowShouldClose(window)) {
        Init::updateDeltaTime();
        Init::processInput(window);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        if (GlobalVariables::gameState == GAME) {
            renderGame(window,game,init,mainMenu);
            renderSkybox(skyboxShader, game);
            handleReturnToMenu(mainMenu);
        } else if(GlobalVariables::gameState == MENU || GlobalVariables::returnToMenuClicked) {
            Init::processInputForPreGame(window,game.chosenCarIndex);
            renderPreGame(game,mainMenu);
        }
        // Flip Buffers and Draw
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    MainMenu::cleanImGui();
    glfwTerminate();
    return EXIT_SUCCESS;
}