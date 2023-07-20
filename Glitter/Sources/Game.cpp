//
// Created by Elena on 09.7.2023.
//

#include "Game.h"



Game::Game(const std::string& shaderVertPath, const std::string& shaderFragPath,
           const std::string& skyVertPath, const std::string& skyFragPath, const std::string& modelPath)
        :ourShader(shaderVertPath.c_str(),shaderFragPath.c_str()),
        skyboxShader(skyVertPath.c_str(),skyFragPath.c_str()),
         car(modelPath),
         camera(glm::vec3(0.0f, 0.0f, 50.0f))
{
    rotationAngle = 0.0f;
    deltaTime = 0.0f;
    lastFrame = 0.0f;
    ambientS = 0.5;
    diffuseS = 1.5;
    specularS = 0.3;
     GLfloat vertices[] = {
            //    Position + Normal + TextOrd
            114.5f,  -1.7f, 111.5f,   0.0f, 0.0f, 0.0f,
            114.5f,  -1.7f, -111.5f,  0.0f, 1.0f, 0.0f,
            -112.0f, -1.7f, 111.5f,   0.0f, 0.0f, 1.0f,
            -112.0f, -1.7f, -111.5f,  0.0f, 1.0f, 1.0f,

    };
    GLuint indices[] = {
            0, 1, 2, 3,
    };

    GLfloat skyboxVertices[] = {
            // Positions
            -1.0f,  1.0f, -1.0f,
            -1.0f, -1.0f, -1.0f,
            1.0f, -1.0f, -1.0f,
            1.0f, -1.0f, -1.0f,
            1.0f,  1.0f, -1.0f,
            -1.0f,  1.0f, -1.0f,

            -1.0f, -1.0f,  1.0f,
            -1.0f, -1.0f, -1.0f,
            -1.0f,  1.0f, -1.0f,
            -1.0f,  1.0f, -1.0f,
            -1.0f,  1.0f,  1.0f,
            -1.0f, -1.0f,  1.0f,

            1.0f, -1.0f, -1.0f,
            1.0f, -1.0f,  1.0f,
            1.0f,  1.0f,  1.0f,
            1.0f,  1.0f,  1.0f,
            1.0f,  1.0f, -1.0f,
            1.0f, -1.0f, -1.0f,

            -1.0f, -1.0f,  1.0f,
            -1.0f,  1.0f,  1.0f,
            1.0f,  1.0f,  1.0f,
            1.0f,  1.0f,  1.0f,
            1.0f, -1.0f,  1.0f,
            -1.0f, -1.0f,  1.0f,

            -1.0f,  1.0f, -1.0f,
            1.0f,  1.0f, -1.0f,
            1.0f,  1.0f,  1.0f,
            1.0f,  1.0f,  1.0f,
            -1.0f,  1.0f,  1.0f,
            -1.0f,  1.0f, -1.0f,

            -1.0f, -1.0f, -1.0f,
            -1.0f, -1.0f,  1.0f,
            1.0f, -1.0f, -1.0f,
            1.0f, -1.0f, -1.0f,
            -1.0f, -1.0f,  1.0f,
            1.0f, -1.0f,  1.0f
    };
    glGenVertexArrays(1,&VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    // !!! Bind the Vertex Array Object first, then bind and set vertex buffer(s) and attribute pointer(s).
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

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
    glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)nullptr);
    glBindVertexArray(0);

    string right= string(SKY_DIR)+"\\skybox\\right.png";
    string left= string(SKY_DIR)+"\\skybox\\left.png";
    string top= string(SKY_DIR)+"\\skybox\\top.png";
    string bottom= string(SKY_DIR)+"\\skybox\\bottom.png";
    string back= string(SKY_DIR)+"\\skybox\\back.png";
    string front= string(SKY_DIR)+"\\skybox\\front.png";
    vector<const GLchar*> faces;
    faces.push_back(right.c_str());
    faces.push_back(left.c_str());
    faces.push_back(top.c_str());
    faces.push_back(bottom.c_str());
    faces.push_back(back.c_str());
    faces.push_back(front.c_str());
    cubemapTexture = loadCubemap(faces);
//    string texture0=string(SKY_DIR)+"\\textures\\text0.jpg";
//    string texture1=string(SKY_DIR)+"\\textures\\text1.jpg";
//    string texture2=string(SKY_DIR)+"\\textures\\text2.jpg";
//     planeText[0] = const_cast<char*>(texture0.c_str());
//     planeText[1] = const_cast<char*>(texture1.c_str());
//     planeText[2] = const_cast<char*>(texture2.c_str());



    ourShader.setInt("material.diffuse",0);
    ourShader.setInt("material.specular",0);


}

void Game::initialStart(){
    string texture0=string(SKY_DIR)+"\\textures\\text0.jpg";
    GLuint  texture = loadTexture(const_cast<GLchar *>(texture0.c_str()));

    auto currentFrame = static_cast<float>(glfwGetTime());
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;

    rotationAngle += 45.0f * deltaTime; // Adjust the rotation speed as desired
    // Background Fill Color
    glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    glBindTexture(GL_TEXTURE_2D, texture);
    ourShader.use();
    glm::mat4 model = glm::mat4(1.0f);
    ourShader.setFloat("model",1);
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_INT, nullptr);
    //
    ourShader.setVec3("viewPos",camera.position());
    // Set material properties
    ourShader.setFloat("material.shininess",32.0f);
    ourShader.setVec3("dir.direction", lightDirection().x,lightDirection().y,lightDirection().z);
    ourShader.setVec3("dirLight.ambient", ambientS,ambientS,ambientS);
    ourShader.setVec3("dirLight.diffuse", diffuseS,diffuseS,diffuseS);
    ourShader.setVec3("dirLight.specular", specularS,specularS,specularS);
    ourShader.setVec3("spotLight.position",camera.position());
    ourShader.setVec3("spotLight.ambient",1.0f, 1.0f, 1.0f);
    ourShader.setVec3("spotLight.diffuse",1.0f, 1.0f, 1.0f);
    ourShader.setVec3("spotLight.specular",1.0f, 1.0f, 1.0f);
    ourShader.setFloat("spotLight.constant",1.0f);
    ourShader.setFloat("spotLight.linear",0.9f);
    ourShader.setFloat("spotLight.quadratic",0.032);
    ourShader.setFloat("spotLight.cuOff",glm::cos(glm::radians(12.5f)));
    ourShader.setFloat("spotLight.outerCutOff",glm::cos(glm::radians(15.0f)));
    //
    glm::mat4 projection = glm::perspective(glm::radians(camera.zoom()),
                                            (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
    glm::mat4 view = camera.GetViewMatrix();
    ourShader.setMat4("projection", projection);
    ourShader.setMat4("view", view);

    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(0.0f, -3.0f, 0.0f));
    model = glm::scale(model, glm::vec3(7.0f, 7.0f, 7.0f));
    model = glm::rotate(model, glm::radians(rotationAngle),
                        glm::vec3(0.0f, 1.0f, 0.0f)); // Rotate around the y-axis
    ourShader.setMat4("model", model);

    car.Draw(ourShader);

    //we can draw more models

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

void Game::start(){

}
void Game::quit(GLFWwindow* window){
    glfwSetWindowShouldClose(window, GLFW_TRUE);
}
void Game::settings(){

}

void Game::processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
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
