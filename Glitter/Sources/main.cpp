#include "init.h"

#include "MainMenu.h"

#include <iostream>
using namespace std;


int main() {
    const string vertPath= string(SHADER_DIR)+"\\default.vert";
    const string fragmentPath= string(SHADER_DIR)+ "\\default.frag";
    const string skyVertPath= string(SHADER_DIR)+"\\skybox.vert";
    const string skyFragPath= string(SHADER_DIR)+"\\skybox.frag";
    const string heightVertPath= string(SHADER_DIR)+"\\terrian.vert";
    const string heightFragPath= string(SHADER_DIR)+"\\terrian.frag";
    const string modelPath= string(MODEL_DIR)+ "\\car\\050 Low Poly Camaro.obj";

    GLFWwindow* mWindow = initializeWindow();
    MainMenu menu(mWindow,vertPath,fragmentPath
                  ,skyVertPath,skyFragPath
            //  heightVertPath,heightFragPath
                  ,modelPath);
    Shader mapShader(heightVertPath.c_str(),heightFragPath.c_str());

    const string mapPath= string(SKY_DIR)+"\\terrian\\iceland_heightmap.png";
    int width, height, nrChannels;
    unsigned char *data = stbi_load(mapPath.c_str(), &width, &height, &nrChannels, 0);
    if (data)
    {
        std::cout << "Loaded heightmap of size " << height << " x " << width << std::endl;
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }

    menu.initializeImGui();
    std::vector<float> vertices;
    float yScale = 64.0f / 256.0f, yShift = 16.0f;
    int rez = 1;
    unsigned bytePerPixel = nrChannels;
    for(int i = 0; i < height; i++)
    {
        for(int j = 0; j < width; j++)
        {
            unsigned char* pixelOffset = data + (j + width * i) * bytePerPixel;
            unsigned char y = pixelOffset[0];

            // vertex
            vertices.push_back( -height/2.0f + height*i/(float)height );   // vx
            vertices.push_back( (int) y * yScale - yShift);   // vy
            vertices.push_back( -width/2.0f + width*j/(float)width );   // vz
        }
    }
    std::cout << "Loaded " << vertices.size() / 3 << " vertices" << std::endl;
    stbi_image_free(data);

    std::vector<unsigned> indices;
    for(unsigned i = 0; i < height-1; i += rez)
    {
        for(unsigned j = 0; j < width; j += rez)
        {
            for(unsigned k = 0; k < 2; k++)
            {
                indices.push_back(j + width * (i + k*rez));
            }
        }
    }
    std::cout << "Loaded " << indices.size() << " indices" << std::endl;

    const int numStrips = (height-1)/rez;
    const int numTrisPerStrip = (width/rez)*2-2;
    std::cout << "Created lattice of " << numStrips << " strips with " << numTrisPerStrip << " triangles each" << std::endl;
    std::cout << "Created " << numStrips * numTrisPerStrip << " triangles total" << std::endl;

    // first, configure the cube's VAO (and terrainVBO + terrainIBO)
    unsigned int terrainVAO, terrainVBO, terrainIBO;
    glGenVertexArrays(1, &terrainVAO);
    glBindVertexArray(terrainVAO);

    glGenBuffers(1, &terrainVBO);
    glBindBuffer(GL_ARRAY_BUFFER, terrainVBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), &vertices[0], GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glGenBuffers(1, &terrainIBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, terrainIBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned), &indices[0], GL_STATIC_DRAW);

    menu.show();
   //    Light light;

    enum GameState { MENU, GAME };
    GameState gameState = MENU;
    bool stateChanged = false;

    while (!glfwWindowShouldClose(mWindow)) {
        menu.game.processInput(mWindow);
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // be sure to activate shader when setting uniforms/drawing objects
        mapShader.use();

        // view/projection transformations
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100000.0f);
        glm::mat4 view = camera.GetViewMatrix();
        mapShader.setMat4("projection", projection);
        mapShader.setMat4("view", view);

        // world transformation
        glm::mat4 model = glm::mat4(1.0f);
        mapShader.setMat4("model", model);

        // render the cube
        glBindVertexArray(terrainVAO);
//        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        for(unsigned strip = 0; strip < numStrips; strip++)
        {
            glDrawElements(GL_TRIANGLE_STRIP,   // primitive type
                           numTrisPerStrip+2,   // number of indices to render
                           GL_UNSIGNED_INT,     // index data type
                           (void*)(sizeof(unsigned) * (numTrisPerStrip+2) * strip)); // offset to starting index
        }

//        if (gameState == GAME) {
//            // Game state
//            menu.game.start( mWindow);
//
//            // Check if the game is finished or if the player wants to go back to the main menu
//            if (menu.game.shouldReturnToMenu()) {
//                // Transition back to the main menu
//                gameState = MENU;
//                menu.show();
//            }
//        } else {
//            // Menu state
//
//            // Show the main menu
//            menu.game.initialStart();
//            menu.renderImGui();
//
//            // Check if the "Start Game" button is clicked in the main menu
//            if (menu.game.gameStarted) {
//                // Transition to the actual game
//                gameState = GAME;
//                menu.game.setRotationAngle();
//            }
//        }

        // Flip Buffers and Draw
        glfwSwapBuffers(mWindow);
        glfwPollEvents();
    }

    MainMenu::cleanImGui();
    glfwTerminate();
    return 0;
}