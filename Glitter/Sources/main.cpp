#include "main.hpp"

void handleReturnToMenu(MainMenu &mainMenu) {
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
    init.processInput(window);
    game.simulation.updateMovements();
    // Step physics forward
    game.simulation.dynamicsWorld->stepSimulation((
       GlobalVariables::deltaTime < GlobalVariables::maxSecPerFrame ?
       GlobalVariables::deltaTime : GlobalVariables::maxSecPerFrame),10);

    game.updateCameraPosition();
    game.transform();
}
void renderGame(GLFWwindow* window,Game& game,Init& init,MainMenu& mainMenu ){
    setUp(window, game, init, mainMenu);
    // Transforms
    glm::mat4 objModelMatrix;
    glm::mat3 objNormalMatrix;

    GLfloat matrix[16];
    btTransform transform;
    Model *objectModel = nullptr;

    int num_cobjs = game.simulation.dynamicsWorld->getNumCollisionObjects();
    game.startGame(objModelMatrix,objNormalMatrix,num_cobjs,objectModel,matrix,transform);

    mainMenu.renderImGui();
    handleReturnToMenu(mainMenu);
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
        gameState = GAME;
}
int main() {
    Init init;
    GLFWwindow *window = Init::initializeWindow();
    map<const string, string> mapForPaths = Init::initializeShadersAndModelsPaths();

    MainMenu mainMenu(window);
    mainMenu.initializeImGui();
    mainMenu.show();
    Game game(mapForPaths["carVertPath"], mapForPaths["carFragPath"],
              mapForPaths["carOneModelPath"],
              mapForPaths["carTwoModelPath"],
              mapForPaths["carThreeModelPath"],
              mapForPaths["tyre1ModelPath"],
              mapForPaths["tyre2ModelPath"],
              mapForPaths["terrainVertPath"], mapForPaths["terrainFragPath"],
              mapForPaths["terrainModel1Path"], mapForPaths["terrainModel2Path"], mapForPaths["terrainModel3Path"],
              mapForPaths["skyVertPath"], mapForPaths["skyFragPath"]);
    game.initialize();

    while (!glfwWindowShouldClose(window)) {
        Init::updateDeltaTime();
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        if (gameState == GAME) {

            renderGame(window,game,init,mainMenu);

        } else {
            Init:: processInputForPreGame(window, game.chosenCarIndex);
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