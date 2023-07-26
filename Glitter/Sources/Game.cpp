//
// Created by Elena on 09.7.2023.
//

#include <thread>
#include "Game.h"



Game::Game(const std::string& shaderVertPath, const std::string& shaderFragPath,
           const std::string& skyVertPath, const std::string& skyFragPath,
//           const std::string& heightVertPath, const std::string& heightFragPath,
           const std::string& modelPath)
        :ourShader(shaderVertPath.c_str(),shaderFragPath.c_str()),
        skyboxShader(skyVertPath.c_str(),skyFragPath.c_str()),
//        mapShader(heightVertPath.c_str(),heightFragPath.c_str()),
         car(modelPath),
         camera(glm::vec3(67.0f, 627.5f, 169.9f),
                glm::vec3(0.0f, 1.0f, 0.0f),
                -128.1f, -42.4f),
         texture(0),cubemapTexture(0),model(glm::mat4(1.0f)),carAcc(5.0f),carTurn(5.0f), rotationAngle(0.0f),deltaTime( 0.0f),
         lastFrame(0.0f),ambientS(0.5),diffuseS(1.5),specularS (0.3),scale(7.0f),physics()
{
    physics.initPhysics();
    initSkybox();
    initTextures();
//    initMap();
    initShaders();
// Reset the gameStarted state to false in the constructor
    gameStarted = false;
}
void Game::initShaders() const {
    ourShader.setInt("material.diffuse",0);
    ourShader.setInt("material.specular",0);
}
void Game::initSkybox() {
    Skybox entity;
    const std::vector<GLfloat>& vertices=entity.getVertices();
    const std::vector<GLuint>& getIndices=entity.getIndices();
    const std::vector<GLfloat>& skyboxVertices=entity.getSkyboxVertices();
    glGenVertexArrays(1,&VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    // !!! Bind the Vertex Array Object first, then bind and set vertex buffer(s) and attribute pointer(s).
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat), vertices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, getIndices.size() * sizeof(GLuint), getIndices.data(), GL_STATIC_DRAW);

    // Set vertices position pointer
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)nullptr);
    glEnableVertexAttribArray(0);

    // Set vertices color pointer
    glVertexAttribPointer(1, 1, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(4 * sizeof(GLfloat)));
    glEnableVertexAttribArray(2);

    glEnable( GL_LINE_SMOOTH );
    glBindVertexArray(0);

    glGenVertexArrays(1, &skyboxVAO);
    glGenBuffers(1, &skyboxVBO);
    glBindVertexArray(skyboxVAO);
    glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
    glBufferData(GL_ARRAY_BUFFER, skyboxVertices.size()*sizeof(GLfloat), skyboxVertices.data(), GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)nullptr);
    glBindVertexArray(0);
}
void Game::initTextures() {
    string right = string(SKY_DIR) + "\\skybox\\right.png";
    string left = string(SKY_DIR) + "\\skybox\\left.png";
    string top = string(SKY_DIR) + "\\skybox\\top.png";
    string bottom = string(SKY_DIR) + "\\skybox\\bottom.png";
    string back = string(SKY_DIR) + "\\skybox\\back.png";
    string front = string(SKY_DIR) + "\\skybox\\front.png";

    vector<const GLchar*> faces;
    faces.push_back(right.c_str());
    faces.push_back(left.c_str());
    faces.push_back(top.c_str());
    faces.push_back(bottom.c_str());
    faces.push_back(back.c_str());
    faces.push_back(front.c_str());

    cubemapTexture = loadCubemap(faces);

    string texture0=string(SKY_DIR)+"\\textures\\text0.jpg";
    texture = loadTexture(const_cast<GLchar *>(texture0.c_str()));
}
void Game::updateDeltaTime()
{
    auto currentFrame = static_cast<float>(glfwGetTime());
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;
}
void Game::setUniforms() {
    ourShader.setVec3("viewPos",camera.Position);
    // Set material properties
    ourShader.setFloat("material.shininess",32.0f);
    ourShader.setVec3("dir.direction", lightDirection().x,lightDirection().y,lightDirection().z);
    ourShader.setVec3("dirLight.ambient", ambientS,ambientS,ambientS);
    ourShader.setVec3("dirLight.diffuse", diffuseS,diffuseS,diffuseS);
    ourShader.setVec3("dirLight.specular", specularS,specularS,specularS);
    ourShader.setVec3("spotLight.position",camera.Position);
    ourShader.setVec3("spotLight.ambient",1.0f, 1.0f, 1.0f);
    ourShader.setVec3("spotLight.diffuse",1.0f, 1.0f, 1.0f);
    ourShader.setVec3("spotLight.specular",1.0f, 1.0f, 1.0f);
    ourShader.setFloat("spotLight.constant",1.0f);
    ourShader.setFloat("spotLight.linear",0.9f);
    ourShader.setFloat("spotLight.quadratic",0.032);
    ourShader.setFloat("spotLight.cuOff",glm::cos(glm::radians(12.5f)));
    ourShader.setFloat("spotLight.outerCutOff",glm::cos(glm::radians(15.0f)));

}
void Game::renderSkybox(){
    glDepthFunc(GL_LEQUAL);  // Change depth function so depth test passes when values are equal to depth buffer's content
    skyboxShader.use();
    view = glm::mat4(glm::mat3(camera.GetViewMatrix()));    // Remove any translation component of the view matrix
    skyboxShader.setMat4("view",view);
    skyboxShader.setMat4("projection",projection);
    // skybox cube
    glBindVertexArray(skyboxVAO);
    glActiveTexture(GL_TEXTURE0);
    skyboxShader.setInt("skybox",0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);
    glDepthFunc(GL_LESS); // Set depth function back to default
}
void Game::initialStart(){

    updateDeltaTime();

    if (!gameStarted) {
        // The game just started, reset the rotation angle to 0 and set the gameStarted flag to true
        rotationAngle += 45.0f * deltaTime; // Adjust the rotation speed as desired
        // Background Fill Color
        glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glBindTexture(GL_TEXTURE_2D, texture);
        ourShader.use();

        ourShader.setFloat("model", 1);

        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_INT, nullptr);

        setUniforms();

        projection = glm::perspective(glm::radians(camera.Zoom),
                                      (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        view = camera.GetViewMatrix();
        ourShader.setMat4("projection", projection);
        ourShader.setMat4("view", view);

        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, -3.0f, 0.0f));
        model = glm::scale(model, glm::vec3(scale));
        model = glm::rotate(model, glm::radians(rotationAngle),
                            glm::vec3(0.0f, 1.0f, 0.0f)); // Rotate around the y-axis
        ourShader.setMat4("model", model);

        car.Draw(ourShader);

        //we can draw more models
        renderSkybox();

        if (rotationAngle >= 360.0f) {
            rotationAngle = 0.0f;
        }
    }
}
void Game::initMap(){
    const string mapPath= string(SKY_DIR)+"\\terrian\\iceland_heightmap.png";
    unsigned char *data = stbi_load(mapPath.c_str(), &width, &height, &nrChannels, 0);
    if (data)
    {
        std::cout << "Loaded heightmap of size " << height << " x " << width << std::endl;
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    std::vector<float> vertices;
    float yScale = 64.0f / 256.0f;
    float yShift = 16.0f;

    unsigned bytePerPixel = nrChannels;

    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------

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
    const unsigned int RESTART_INDEX = std::numeric_limits<unsigned int>::max();


    for(int i = 0; i < height-1; i += rez)
    {
        for(int j = 0; j < width; j += rez)
        {
            for(int k = 0; k < 2; k++)
            {
                indices.push_back(j + width * (i + k*rez));
            }
            indices.push_back(RESTART_INDEX);
        }
    }
    std::cout << "Loaded " << indices.size() << " indices" << std::endl;


    std::cout << "Created lattice of " << numStrips << " strips with " << numTrisPerStrip << " triangles each" << std::endl;
    std::cout << "Created " << numStrips * numTrisPerStrip << " triangles total" << std::endl;

    // first, configure the cube's VAO (and terrainVBO + terrainIBO)

    glGenVertexArrays(1, &terrainVAO);
    glBindVertexArray(terrainVAO);

    glGenBuffers(1, &terrainVBO);
    glBindBuffer(GL_ARRAY_BUFFER, terrainVBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), &vertices[0], GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)nullptr);
    glEnableVertexAttribArray(0);

    glGenBuffers(1, &terrainIBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, terrainIBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned), &indices[0], GL_STATIC_DRAW);

    // Unbind the VAO to avoid accidental changes
    glBindVertexArray(0);

    // Enable the depth test
    glEnable(GL_DEPTH_TEST);
}
void Game::renderTerrian(){
    //mapShader.use();

    // view/projection transformations
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(0.0f, -3.0f, 0.0f)); // Adjust the position of the terrain
    model = glm::scale(model, glm::vec3(scale)); // Adjust the scale of the terrain
  //  mapShader.setMat4("model", model);
    glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
    glm::mat4 view = camera.GetViewMatrix();
    //mapShader.setMat4("projection", projection);
  //  mapShader.setMat4("view", view);

    // render the cube
    glBindVertexArray(terrainVAO);
//        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    for( int strip = 0; strip < numStrips;  strip++)
    {
        glDrawElements(GL_TRIANGLE_STRIP,   // primitive type
                       numTrisPerStrip+2,   // number of indices to render
                       GL_UNSIGNED_INT,     // index data type
                       (void*)(sizeof(unsigned) * (numTrisPerStrip+2) * strip)); // offset to starting index
    }
  }

void Game::start(GLFWwindow* window) {
    // Calculate delta time
    auto currentFrame = static_cast<float>(glfwGetTime());
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;

    // Calculate FPS
    static int frames = 0;
    static double totalTime = 0.0;
    frames++;
    totalTime += deltaTime;
    if (totalTime >= 1.0) {
        double fps = static_cast<double>(frames) / totalTime;
        std::cout << "FPS: " << fps << std::endl;
        frames = 0;
        totalTime = 0.0;
    }

    // Process input
    processInput(window);


    // Update camera position and orientation to follow the car
    glm::vec3 carPosition = glm::vec3(carTurn, -12.0f, carAcc);
    glm::vec3 cameraOffset = glm::vec3(0.0f, 5.0f, -10.0f); // Adjust the offset as needed
    glm::vec3 cameraPosition = carPosition + cameraOffset;
    glm::vec3 cameraTarget = carPosition; // Look at the car's position

    // Clamp camera position to stay within window bounds
    const float halfWindowWidth = SCR_WIDTH / 2.0f;
    const float halfWindowHeight = SCR_HEIGHT / 2.0f;
    const float minX = -halfWindowWidth;
    const float maxX = halfWindowWidth;
    const float minY = -halfWindowHeight;
    const float maxY = halfWindowHeight;

    cameraPosition.x = glm::clamp(cameraPosition.x, minX, maxX);
    cameraPosition.y = glm::clamp(cameraPosition.y, minY, maxY);


    // Clear the screen
    glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Set the view matrix for all shaders
    view = glm::lookAt(cameraPosition, cameraTarget, glm::vec3(0.0f, 1.0f, 0.0f));
    ourShader.setMat4("view", view);
    skyboxShader.setMat4("view", view);
//    mapShader.setMat4("view", view);
//    // Draw the terrain
//    renderTerrian();


    // Draw the scene
    glBindTexture(GL_TEXTURE_2D, texture);
    ourShader.use();
    model = glm::mat4(1.0f);
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_INT, nullptr);

    // Set uniforms for lighting
    setUniforms();

    // Draw the car
    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(carTurn, -12.0f, carAcc));
    model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    model = glm::scale(model, glm::vec3(scale));
    ourShader.setMat4("model", model);
    car.Draw(ourShader);

    // Draw the skybox
    renderSkybox();
    // Swap buffers and poll events
    glfwSwapBuffers(window);
    glfwPollEvents();
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
//    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS){
//        carAcc-=1.0f;
//        carAcc = glm::clamp(carAcc, -33.0f, -4.0f);
//    }
//    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS){
//        carAcc+=1.0f;
//        carAcc = glm::clamp(carAcc, -33.0f, -4.0f);
//    }
//    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS){
//        carTurn+=1.0f;
//        carTurn = glm::clamp(carTurn, -17.0f, 15.0f);
//    }
//    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS){
//        carTurn-=1.0f;
//        carTurn = glm::clamp(carTurn, -17.0f, 15.0f);
//    }
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, deltaTime);




}
GLuint  Game::loadCubemap(std::vector<const GLchar*> faces)
{
    GLuint textureID;
    glGenTextures(1, &textureID);

    int width, height, channels;
    unsigned char* image;

    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);
    for (GLuint i = 0; i < faces.size(); i++)
    {
        image = stbi_load(faces[i], &width, &height, &channels, 0);
        GLint  format;
        switch (channels)
        {
            case 1: format = GL_ALPHA;     break;
            case 2: format = GL_LUMINANCE; break;
            case 3: format = GL_RGB;       break;
            case 4: format = GL_RGBA;      break;
            default: format = GL_RGB;      break;
        }
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, image);
        stbi_image_free(image);
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

    return textureID;
}
GLuint  Game::loadTexture(GLchar* path)
{
    GLuint textureID;
    glGenTextures(1, &textureID);
    int width, height, channels;
    unsigned char* image = stbi_load(path, &width, &height, &channels, 0);
    GLint format;
    switch (channels)
    {
        case 1: format = GL_ALPHA;     break;
        case 2: format = GL_LUMINANCE; break;
        case 3: format = GL_RGB;       break;
        case 4: format = GL_RGBA;      break;
        default: format = GL_RGB;      break;
    }
    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, image);
    glGenerateMipmap(GL_TEXTURE_2D);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);
    stbi_image_free(image);
    return textureID;
}

glm::vec3 Game::lightDirection() {
    return {-10.2f, 0.0f, -5.0f};
}

void Game::setRotationAngle(){
    rotationAngle = 0.0f;
}

